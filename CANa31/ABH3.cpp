//概要
//	ABH3用CAN制御クラス
//
//履歴
//	2020/09/xx	yo0043		1st release
//	2020/09/29	yo0043		2nd release
//							送信長を8バイト未満にする必要が有る為、幾つかの関数に送信長を追加
//							マルチパケットのデフォルト要求数を 0xff -> 0x08 に変更
//							マルチパケットを途中で中止する場合にABORTを発行する様に修正
//							（送信に失敗した場合が主な原因の為、ABORTが実際に送信されるかは時の運）
//	2021/02/15	yo0043		3rd release
//							コメント追加のみでプログラムに変更無し
//

#include "stdafx.h"
#include <memory.h>		//memset,
#include "Abh3.h"

//コンストラクタ
CAbh3::CAbh3(CCanIF* pDeviceClass /* NULL */)
	{
	//内部変数の初期化
	::memset(&m_var,0,sizeof(CANABH3_VAR));

	//通信制御用セマフォ構築
	m_var.hTerm = ::CreateSemaphore(NULL,1,1,NULL);

	//出力段のクラスを保存
	m_var.pDeviceClass = pDeviceClass;
	}

//デストラクタ
CAbh3::~CAbh3()
	{
	//インターフェースを閉じる（開いている場合が有る為）
	CloseInterface();

	//通信制御用セマフォの開放
	::CloseHandle(m_var.hTerm);
	m_var.hTerm = NULL;
	}

//================================================================================
//ユーザーが利用可能な関数（基本要素）
//================================================================================

//使用可能なCANインターフェース数を取得
int32_t CAbh3::GetInterfaceCount(void)
	{
	//デバイスが未登録？
	if(!IsValidDevice())
		return(0);	//デバイス無し扱い
	return(m_var.pDeviceClass->OnGetInterfaceCount());
	}

//指定インターフェースを開く
int32_t CAbh3::OpenInterface(int32_t nDeviceNum)
	{
	//CANインターフェースを開いている場合が有るので閉じる
	CloseInterface();

	//デバイスが未登録？
	if(!IsValidDevice())
		return(-1);	//エラー

	//CANインターフェースを開く
	int32_t nResult = m_var.pDeviceClass->OnOpenInterface(nDeviceNum);
	//正常終了？
	if(nResult == 0)
		{
		//開いたCANインターフェース情報を保存
		m_var.nOpenDevice = nDeviceNum + 1;
		//デバイスにホストのIDを渡して、無視する対象とさせる
		m_var.pDeviceClass->SetIgnoreID(GetHostID());
		}
	return(nResult);
	} 

//開いたインターフェースを閉じる
void CAbh3::CloseInterface()
	{
	//デバイス登録有り？
	if(IsValidDevice())
		{
		//CANインターフェースを閉じる
		m_var.pDeviceClass->OnCloseInterface();
		}
	//開いているCANインターフェース情報を解除
	m_var.nOpenDevice = 0;
	}

//現在開いているインターフェース番号を取得
int32_t CAbh3::GetCurrentInterface()
	{
	return(m_var.nOpenDevice - 1);
	}

//現在インターフェースを開いているか？
int32_t CAbh3::IsOpenInterface()
	{
	if(GetCurrentInterface() >= 0)
		return(1);
	return(0);
	}

//インターフェースを開くタイムアウト時間を設定
void CAbh3::SetOpenTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_var.pDeviceClass->SetTimeout_Open(nTimeoutMS);
	}

//送信タイムアウト時間を設定
void CAbh3::SetSendTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_var.pDeviceClass->SetTimeout_Send(nTimeoutMS);
	}

//受信タイムアウト時間を設定
void CAbh3::SetRecvTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_var.pDeviceClass->SetTimeout_Recv(nTimeoutMS);
	}

//通信対象ABH3のアドレスを設定
void CAbh3::SetTargetID(uint8_t nAdrs)
	{
	m_var.config.nTargetAdrs = nAdrs;
	}

//通信対象ABH3のアドレスを取得
uint8_t CAbh3::GetTargetID()
	{
	return(m_var.config.nTargetAdrs);
	}

//通信ホストのアドレスを設定
void CAbh3::SetHostID(uint8_t nAdrs)
	{
	m_var.config.nHostAdrs = nAdrs;
	}

//通信ホストのアドレスを取得
uint8_t CAbh3::GetHostID()
	{
	return(m_var.config.nHostAdrs);
	}

//通信速度を指定
void CAbh3::SetBaudrate(uint32_t nBaudrateKbps)
	{
	//概要
	//	通信ボーレート指定
	//パラメータ
	//	nBaudrateKbps	ボーレートを[kbps]単位で指定（デバイス依存なので注意）

	//デバイスが登録済みならボーレート設定
	if(IsValidDevice())
		m_var.pDeviceClass->SetBaudrate(nBaudrateKbps);
	}

//設定した通信速度を取得
uint32_t CAbh3::GetBaudrate(void)
	{
	//デバイスが未登録？
	if(!IsValidDevice())
		return(0);
	return(m_var.pDeviceClass->GetBaudrate());
	}

//================================================================================
//ユーザーが利用可能な関数（ABH3要素）
//================================================================================

//指令の初期化
int32_t CAbh3::abh3_can_init()
	{
	//戻り値
	int32_t nResult = -1;

	//通信排他制御用のセマフォを取得
	if(Lock() == 0)
		{
		//インターフェースを開いている？
		if(IsOpenInterface())
			{
			//設定値を格納
			m_var.lastdata.send.nOrderAY = 0;
			m_var.lastdata.send.nOrderBX = 0;
			m_var.lastdata.send.nInputBit = 0;

			//シングルパケット(DP0)を構築
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 m_var.lastdata.send.nOrderAY
				,m_var.lastdata.send.nOrderBX
				,m_var.lastdata.send.nInputBit
				);

			//送信先IDを作成
			uint32_t nSendID = CreateID(0x00ef0000);

			//受信用バッファ（使わない）
			CANABH3_RESULT result;

			//受信バッファのクリア
			ClearRecvBuffer();

			//送信
			nResult = CanTermSingle(nSendID,pPacket,&result,8);

			//開放
			CCan1939::FreeBuffer(pPacket);
			}
		//通信排他制御用のセマフォを開放
		Unlock();
		}

	//完了
	return(nResult);
	}

//指令の送信（軸別）
int32_t CAbh3::abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr)
	{
	//abh3_can_cmdを利用する

	//実行
	return(abh3_can_cmd(cmd,m_var.lastdata.send.nOrderBX,pPtr));
	}
int32_t CAbh3::abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr)
	{
	//abh3_can_cmdを利用する

	//実行
	return(abh3_can_cmd(m_var.lastdata.send.nOrderAY,cmd,pPtr));
	}

//指令の送信（同時）
int32_t CAbh3::abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr)
	{
	//戻り値
	int32_t nResult = -1;

	//通信排他制御用のセマフォを取得
	if(Lock() == 0)
		{
		//インターフェースを開いている？
		if(IsOpenInterface())
			{
			//設定値を格納
			m_var.lastdata.send.nOrderAY = cmdAY;
			m_var.lastdata.send.nOrderBX = cmdBX;

			//シングルパケット(DP0)を構築
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 m_var.lastdata.send.nOrderAY
				,m_var.lastdata.send.nOrderBX
				,m_var.lastdata.send.nInputBit
				);

			//送信先IDを作成
			uint32_t nSendID = CreateID(0x00ef0000);

			//受信バッファのクリア
			ClearRecvBuffer();

			//送信
			nResult = CanTermSingle(nSendID,pPacket,pPtr,8);

			//開放
			CCan1939::FreeBuffer(pPacket);
			}
		//通信排他制御用のセマフォを開放
		Unlock();
		}

	//構造体に送信要素を書き込み
	SetLastData(pPtr);

	//完了
	return(nResult);
	}

//入力の送信（一括）
int32_t CAbh3::abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr)
	{
	//戻り値
	int32_t nResult = -1;

	//通信排他制御用のセマフォを取得
	if(Lock() == 0)
		{
		//インターフェースを開いている？
		if(IsOpenInterface())
			{
			//設定値を格納
			m_var.lastdata.send.nInputBit = (data & ~mask) | (data & mask);

			//シングルパケット(DP0)を構築
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 m_var.lastdata.send.nOrderAY
				,m_var.lastdata.send.nOrderBX
				,m_var.lastdata.send.nInputBit
				);

			//送信先IDを作成
			uint32_t nSendID = CreateID(0x00ef0000);

			//受信バッファのクリア
			ClearRecvBuffer();

			//送信
			nResult = CanTermSingle(nSendID,pPacket,pPtr,8);

			//開放
			CCan1939::FreeBuffer(pPacket);
			}
		//通信排他制御用のセマフォを開放
		Unlock();
		}

	//構造体に送信要素を書き込み
	SetLastData(pPtr);

	//完了
	return(nResult);
	}

//入力の送信（ビット）
int32_t CAbh3::abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr)
	{
	//戻り値
	int32_t nResult = -1;

	//通信排他制御用のセマフォを取得
	if(Lock() == 0)
		{
		//インターフェースを開いている？
		if(IsOpenInterface())
			{
			//設定値を格納
			if(data)
				m_var.lastdata.send.nInputBit |= (1 << num);
			else
				m_var.lastdata.send.nInputBit &= ~(1 << num);

			//シングルパケット(DP0)を構築
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 m_var.lastdata.send.nOrderAY
				,m_var.lastdata.send.nOrderBX
				,m_var.lastdata.send.nInputBit
				);

			//送信先IDを作成
			uint32_t nSendID = CreateID(0x00ef0000);

			//受信バッファのクリア
			ClearRecvBuffer();

			//送信
			nResult = CanTermSingle(nSendID,pPacket,pPtr,8);

			//開放
			CCan1939::FreeBuffer(pPacket);
			}
		//通信排他制御用のセマフォを開放
		Unlock();
		}

	//構造体に送信要素を書き込み
	SetLastData(pPtr);

	//完了
	return(nResult);
	}

//積算値のリクエスト
int32_t CAbh3::abh3_can_reqPulse(pCANABH3_RESULT pPtr)
	{
	//戻り値
	int32_t nResult = -1;

	//通信排他制御用のセマフォを取得
	if(Lock() == 0)
		{
		//インターフェースを開いている？
		if(IsOpenInterface())
			{
			//シングルパケット(DP1)を構築
			uint8_t* pPacket = CCan1939::CreateSGL1();

			//送信先IDを作成
			uint32_t nSendID = CreateID(0x01ef0000);

			//受信バッファのクリア
			ClearRecvBuffer();

			//送信
			nResult = CanTermSingle(nSendID,pPacket,pPtr,0);

			//開放
			CCan1939::FreeBuffer(pPacket);
			}
		//通信排他制御用のセマフォを開放
		Unlock();
		}

	//構造体に送信要素を書き込み
	SetLastData(pPtr);

	//完了
	return(nResult);
	}

//ブロードキャストパケットのリクエスト
int32_t CAbh3::abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr)
	{
	//戻り値
	int32_t nResult = -1;

	//通信排他制御用のセマフォを取得
	if(Lock() == 0)
		{
		//インターフェースを開いている？
		if(IsOpenInterface())
			{
			//シグネチャを構築
			uint32_t nSign = CreateBrdID(num);

			//ブロードキャストパケット(num)を構築
			uint8_t* pPacket = CCan1939::CreateBRD(nSign);

			//送信先IDを作成
			uint32_t nSendID = CreateID(0x00ea0000);

			//受信バッファのクリア
			ClearRecvBuffer();

			//受信対象のIDを構築
			//	（送信先から発信され、指定番号が受信先になっている物が対象）
			uint32_t nOnlyID = 0x00ff0000 | (num << 8) | GetTargetID();

			//送信
			nResult = CanTermSingle(nSendID,nOnlyID,pPacket,pPtr,3);

			//開放
			CCan1939::FreeBuffer(pPacket);
			}
		//通信排他制御用のセマフォを開放
		Unlock();
		}

	//構造体に送信要素を書き込み
	SetLastData(pPtr);

	//完了
	return(nResult);
	}

//マルチパケットによるTelABH3パケットの送受信
int32_t CAbh3::abh3_can_trans(char* sbuf,char*& rbuf,size_t& rbuflen)
	{
	//概要
	//	sbuf		送信データが格納されたポインタ
	//	rbuf		受信データを格納するポインタ変数への参照
	//	rbuflen		受信データバッファ長
	//戻り値
	//	0			正常
	//	0以外		異常
	//注意点
	//	rbufは呼び出し元で開放(delete)する必要があります
	//	通信が成功又は失敗するまで処理が戻りません
	//使用例
	//	char* sbuf = "CP000095";
	//	char* rbuf = NULL;
	//	size_t rbuflen = 0;
	//	if(abh3_can_trans(sbuf,rbuf,rbuflen) == 0)
	//		{
	//		//正常な場合の処理
	//		printf("%s\n",rbuf);
	//		}
	//	else
	//		{
	//		//異常な場合の処理
	//		printf("Error\n");
	//		}
	//	if(rbuf)
	//		delete rbuf;

	//戻り値
	int32_t nResult = -1;

	//通信排他制御用のセマフォを取得
	if(Lock() == 0)
		{
		//インターフェースを開いている？
		if(IsOpenInterface())
			{
			//一時バッファ構築
			//2020.10.09 yo0043 以下仕様に変更
			//	データ部分が最低9バイトになる様に1つ以上のSYNを追加

			//文字列のサイズを元に、バッファサイズを算出
			size_t nSrcLen = ::strlen(sbuf);
			size_t nBufLen = 0;
			if(nSrcLen < 9)
				nBufLen = 9;
			else
				nBufLen = nSrcLen + 1;

			//終端分を考慮してバッファを構築
			char* pTmp = new char[nBufLen + 1]();	//初期化付きで確保（終端の為）

			//終端を除いてSYN(0x16)で埋める
			::memset(pTmp,0x16,nBufLen);

			//元文字列を先頭から文字数分だけコピーする
			::memcpy(pTmp,sbuf,nSrcLen);

			//型変換
			uint8_t* pSrc = (uint8_t*)pTmp;
			uint8_t* pDst = 0;
			uint32_t nDstSize = 0;

			//送受信
			nResult = CanTermSendMulti(pSrc,uint32_t(nBufLen),pDst,nDstSize);
			if(nResult == 0)
				{
				rbuf = (char*)pDst;
				rbuflen = (size_t)nDstSize;
				}
			//一時バッファ開放
			delete pTmp;
			}
		//通信排他制御用のセマフォを開放
		Unlock();
		}

	//完了
	return(nResult);
	}

//================================================================================
//内部用
//================================================================================

//8[bytes]送受信
int32_t CAbh3::CanSend8(uint32_t nSendID,uint8_t* pSendData,uint8_t nLength)
	{
	//概要
	//	CANインターフェースに対してIDを指定してデータを送信
	//パラメータ
	//	nSendID		CAN-ID(29bit)
	//	pSendData	送信データへのポインタ
	//戻り値
	//	0			正常終了
	//	0以外		異常終了
	//注意点
	//	排他制御はこの関数の呼び出し元が行う事

	//戻り値
	int32_t nResult = -1;

	//デバイスクラスが登録済みなら送信
	if(m_var.pDeviceClass)
		nResult = m_var.pDeviceClass->OnCanSend(nSendID,pSendData,nLength);

	//完了
	return(nResult);
	}

//受信
int32_t CAbh3::CanRecv8(uint32_t* pRecvID,uint8_t* pRecvData)
	{
	//概要
	//	CANインターフェースからデータを受信
	//パラメータ
	//	pRecvID		受信したCAN-ID(29bit)格納先へのポインタ
	//	pRecvData	受信データ格納先へのポインタ
	//戻り値
	//	0			正常終了
	//	0以外		異常終了
	//注意点
	//	排他制御はこの関数の呼び出し元が行う事

	//戻り値
	int32_t nResult = -1;

	//デバイスクラスが登録済みなら受信
	if(m_var.pDeviceClass)
		nResult = m_var.pDeviceClass->OnCanRecv(pRecvID,pRecvData);

	//完了
	return(nResult);
	}

//シングルパケット送受信
int32_t CAbh3::CanTermSingle(uint32_t nSendID,uint8_t* pSendData,pCANABH3_RESULT pResult,uint8_t nLength)
	{
	//概要
	//	CANインターフェースを使用してシングルパケットを送受信
	//パラメータ
	//	nSendID		送信対象ID
	//	pSendData	送信データへのポインタ
	//	pResult		返答データ格納先へのポインタ
	//戻り値
	//	0			正常終了
	//	0以外		異常終了

	//送信
	int32_t nResult = CanSend8(nSendID,pSendData,nLength);

	//送信成功？
	if(nResult == 0)
		nResult = CanRecv8(&pResult->nID,pResult->u.raw);	//受信
	//
	return(nResult);
	}
int32_t CAbh3::CanTermSingle(uint32_t nSendID,uint32_t nOnlyID,uint8_t* pSendData,pCANABH3_RESULT pResult,uint8_t nLength)
	{
	//概要
	//	CANインターフェースを使用してシングルパケットを送受信
	//パラメータ
	//	nSendID		送信対象ID
	//	nOnlyID		受信対象となる唯一のID
	//	pSendData	送信データへのポインタ
	//	pResult		返答データ格納先へのポインタ
	//戻り値
	//	0			正常終了
	//	0以外		異常終了

	//デバイスが使えない？
	if(!IsValidDevice())
		return(-1);		//送信不可

	//受信対象となる唯一のIDを設定
	m_var.pDeviceClass->SetRecvOnlyID(nOnlyID);

	//送信
	int32_t nResult = CanSend8(nSendID,pSendData,nLength);

	//送信成功？
	if(nResult == 0)
		nResult = CanRecv8(&pResult->nID,pResult->u.raw);	//受信

	//受信対象となる唯一のIDを解除
	m_var.pDeviceClass->SetRecvOnlyID(0);

	//
	return(nResult);
	}

//マルチパケット送信
int32_t CAbh3::CanTermSendMulti(uint8_t* pSendData,uint32_t nSendDataSize,uint8_t*& pRecvData,uint32_t& nRecvDataSize)
	{
	//概要
	//	マルチパケット送信（PC側を起点とする送受信）を行う
	//パラメータ
	//	pSendData		送信データへのポインタ
	//	nSendDataSize	送信データ長[bytes]
	//	pRecvData		受信データを扱うポインタへの参照
	//	nRecvDataSize	受信データ長を格納する変数の参照
	//戻り値
	//	0				正常終了
	//	-11				通信先から中止要求(ABORT)を受信
	//	-12				通信先からセッション重複要求（複数回のRTS）を受信
	//	-13				PC側で送信失敗
	//	-14				PC側で受信失敗（タイムアウト）
	//
	//注意点
	//	関数が正常に終了した場合、pRecvDataには受信データが格納された領域へのポインタが入りますが、
	//	このポインタは呼び出し元で開放する必要が有ります
	//	pSendData末尾にSYN(0x16)を追加してから呼び出す事
	//	(abh3_can_trans関数内で追加している）

	//戻り値
	int32_t nExitCode = 0;

	//
	uint32_t nStage = 0;	//状態遷移処理用ステージ番号
	int32_t nResult = 0;	//汎用の戻り値
	uint8_t nAbort = 0;		//Abort返答時の理由

	//ホスト側から見た送信用IDと受信判定用ID
	uint32_t nSendID		= 0x00ec0000 | (m_var.config.nTargetAdrs << 8) | m_var.config.nHostAdrs;
	uint32_t nSendDTID		= 0x00eb0000 | (m_var.config.nTargetAdrs << 8) | m_var.config.nHostAdrs;
	uint32_t nRecvID		= 0x00ec0000 | (m_var.config.nHostAdrs << 8) | m_var.config.nTargetAdrs;
	uint32_t nRecvDTID		= 0x00eb0000 | (m_var.config.nHostAdrs << 8) | m_var.config.nTargetAdrs;
	uint32_t nID			= 0;	//
	uint8_t nMaxPacket		= 0;	//送信許可パケット数
	uint8_t nPacketNum		= 0;	//パケット番号（1-255)
	uint8_t nTotalPacket	= 0;	//受信時の総パケット数
	//
	while(-1)
		{
		//RTSを送るステージ
		//	PCが送信ノード・ABH3が受信ノードで送信要求を送る
		if(nStage == 0)
			{
			//送信許可パケット数を初期化（ホストの最大値）
			nMaxPacket = 0xff;

			//CM_RTS送信
			uint8_t* pPacket = CCan1939::CreateCMRTS(nSendDataSize,nMaxPacket);
			nResult = CanSend8(nSendID,pPacket,8);
			CCan1939::FreeBuffer(pPacket);
			//送信正常？
			if(nResult == 0)
				nStage = 1;
			else
				{
				//送信失敗
				nExitCode = -13;
				nStage = 99;	//RTS送信時に失敗、何も送信してないのでそのまま終了
				}
			}

		//CTS/EOMA/ABORTを受け取るステージ
		//	PCが送信ノード・ABH3が受信ノードで送信要求(RTS)又はデータ送信(DT)の返答が戻る
		else if(nStage == 1)
			{
			uint8_t* pPacket = CCan1939::CreateBuffer();
			nResult = CanRecv8(&nID,pPacket);
			if(nResult == 0)
				{
				//受信は正常

				//何のパケット？
				uint8_t nType = CCan1939::IsPacket(pPacket);

				//送信要求？
				if(nType == CCan1939::DEF_CTS)
					{
					//ターゲット側から指定された送信許可パケット数を保存
					nMaxPacket = pPacket[1];
					//次に送信するパケット番号を保存
					nPacketNum = pPacket[2];
					//DTを送るステージに遷移
					nStage = 2;
					}
				//完了通知？
				else if(nType == CCan1939::DEF_EOMA)
					{
					//受信完了扱い
					nStage = 3;
					}
				//中止要求？
				else if(nType == CCan1939::DEF_ABORT)
					{
					//中止扱い
					nExitCode = -11;
					nStage = 99;
					}
				//新規要求？
				else if(nType == CCan1939::DEF_RTS)
					{
					//セッション重複扱いでエラー
					nExitCode = -12;
					nAbort = 1;
					nStage = 10;
					}
				//上記以外？
				else
					{
					//上記以外は見なかった事にして再送を待つ
					Sleep(0);
					}
				}
			else
				{
				//受信処理に失敗（タイムアウト）
				nExitCode = -14;
				nAbort = 3;	//Timeout
				nStage = 10;
				}
			//
			CCan1939::FreeBuffer(pPacket);
			}

		//DTを送るステージ
		//	PCが送信ノード・ABH3が受信ノードで1つ前の返答(CTS)で指定されたデータを送る
		//	送る位置(nPacketNum)と個数(nMaxPacket)は、1つ前の返答(CTS)で指定されている
		else if(nStage == 2)
			{
			//要求数が0?
			if(nMaxPacket < 1)
				{
				//最低1つは送る
				nMaxPacket = 1;
				}
			//
			for(uint8_t nLoop = 0;nLoop < nMaxPacket;nLoop++)
				{
				//CM_DTパケットを作成出来たら送信する
				uint8_t* pPacket = CCan1939::CreateCMDT(pSendData,nSendDataSize,nPacketNum);
				if(pPacket)
					nResult = CanSend8(nSendDTID,pPacket,8);
	
				//送る位置を更新
				++nPacketNum;

				//作成したCM_DTパケットを開放
				CCan1939::FreeBuffer(pPacket);

				//送信失敗？
				if(nResult)
					break;	//途中で抜ける
				}

			//CM_DTパケットの送信に失敗？
			if(nResult)
				{
				//送信失敗
				nExitCode = -13;
				nAbort = 2;	//resource不足
				nStage = 10;
				}
			else
				nStage = 1;
			}

		//EOMAを受け取った後のRTS受信ステージ
		//	PCが受信ノード・ABH3が送信ノードに役割変更し、返答(CM_RTS)が届くはずなのでそれを受信
		else if(nStage == 3)
			{
			//受信領域構築
			uint8_t* pPacket = CCan1939::CreateBuffer();
			nResult = CanRecv8(&nID,pPacket);
			//受信は正常？
			if(nResult == 0)
				{
				//何のパケット？
				uint8_t nType = CCan1939::IsPacket(pPacket);
				//CM_RTS?
				if(nType == CCan1939::DEF_RTS)
					{
					//メッセージサイズを取得し、バッファを構築
					nRecvDataSize = CCan1939::Get16L(pPacket,1);
					pRecvData = new uint8_t[nRecvDataSize + 1]();	//終端用に+1して初期化済みで確保する

					//ターゲット側から指定された総パケット数を保存
					nTotalPacket = CCan1939::Get8L(pPacket,3);

					//ターゲット側から指定された送信許可パケット数を保存
					nMaxPacket = pPacket[4];

					//パケット番号を初期化
					nPacketNum = 1;

					//CTSを返答するステージに遷移
					nStage = 4;
					}
				//CM_EOMA?
				else if(nType == CCan1939::DEF_EOMA)
					{
					nStage = 6;
					}
				//CM_ABORT?
				else if(nType == CCan1939::DEF_ABORT)
					{
					//中止扱い
					nExitCode = -11;
					nStage = 99;
					}
				//上記以外？
				else
					{
					//上記以外は見なかった事にして再送を待つ
					Sleep(0);
					}
				}
			else
				{
				//受信処理時に失敗、ABORT送信
				nExitCode = -14;
				nAbort = 3;	//Timeout
				nStage = 10;	
				}
			//
			CCan1939::FreeBuffer(pPacket);
			}
		//CTS返答ステージ
		//	PCが受信ノード・ABH3が送信ノードで、受信要求(CM_RTS)又はデータ(CM_DT)の返答(CM_CTS)が必要なのでそれを送信
		else if(nStage == 4)
			{
			//送信許可パケットはターゲット側が指定した値をそのまま利用

			//CM_CTSパケットを作って送信
			uint8_t nRemainPacket = nMaxPacket - nPacketNum + 1;				//残りのパケット数
			uint8_t* pPacket = CCan1939::CreateCMCTS(nRemainPacket,nPacketNum);
			nResult = CanSend8(nSendID,pPacket,8);
			CCan1939::FreeBuffer(pPacket);
			//CM_CTS送信正常？
			if(nResult == 0)
				nStage = 5;
			else
				{
				//送信失敗
				nExitCode = -13;
				nAbort = 2;	//resource不足
				nStage = 10;
				}
			}

		//DT受信ステージ
		//	PCが受信ノード・ABH3が送信ノードで、要求(CM_CTS)又はデータ(CM_DT)の返答(CM_RTS)が必要なのでそれを送信
		else if(nStage == 5)
			{
			//debug
			if(nMaxPacket < 1)
				{
				nMaxPacket = 1;
				}

			//受信領域構築
			bool bRetry = false;
			uint8_t* pPacket = CCan1939::CreateBuffer();

			//送信されるデータ個数（予定数）の数だけ受信処理を行う
			for(uint8_t nLoop = 0;nLoop < nMaxPacket;nLoop++)
				{
				//受信
				nResult = CanRecv8(&nID,pPacket);
				//受信は正常？
				if(nResult == 0)
					{
					//シーケンス番号が異なる？
					if(pPacket[0] != nPacketNum)
						{
						//要求したのと異なる物が届いたので、再度要求する
						bRetry = true;
						break;
						}
					
					//データ格納（7バイト）
					CCan1939::SetData(pRecvData,nRecvDataSize,CCan1939::packetnum2datapt(nPacketNum),&pPacket[1],7);

					//最終パケット番号？
					if(nPacketNum == nTotalPacket)
						break;	//完了

					//要求したパケット番号で届いたので、次のパケット番号に更新
					++nPacketNum;
					}
				else
					{
					//受信処理に失敗
					nExitCode = -14;
					nAbort = 3;	//Timeout
					nStage = 10;
					break;
					}
				}
			//受信領域開放
			CCan1939::FreeBuffer(pPacket);

			//リトライが必要？
			if(bRetry)
				{
				//パケット番号(nPacketNum)が正しくない箇所から再度データ(CM_DT)を要求する
				nStage = 4;
				}

			//正常？
			else if(nResult == 0)
				{
				if(nPacketNum == nTotalPacket)
					nStage = 6;	//EOMA送信ステージに遷移
				else
					nStage = 4;	//CTS返答に戻る
				}

			else
				{
				//受信失敗、受信ルーチン内で移行先が設定されている為、何もしない
				Sleep(0);
				}
			}

		//EOMA返答ステージ
		else if(nStage == 6)
			{
			//パケットを作って送信
			uint8_t* pPacket = CCan1939::CreateCMEOMA(nTotalPacket * 8,nTotalPacket);
			nResult = CanSend8(nSendID,pPacket,8);
			CCan1939::FreeBuffer(pPacket);
			//送信正常？
			if(nResult == 0)
				{
				//完了ステージに移行
				nStage = 7;
				}
			else
				{
				//送信失敗
				nExitCode = -13;
				nAbort = 2;
				nStage = 10;
				}
			}

		//完了ステージ
		else if(nStage == 7)
			{
			//正常完了
			break;
			}

		//ホスト側から中止要求ステージ
		//	CM_ABORTを送る、nAbortに理由を入れておく(2...リソース不足  3..タイムアウト)
		else if(nStage == 10)
			{
			//ABORTパケットを作る
			uint8_t* pPacket = CCan1939::CreateCMABORT(nAbort);
			nResult = CanSend8(nSendID,pPacket,8);
			CCan1939::FreeBuffer(pPacket);
			//
			nStage = 99;
			}

		//上記以外（エラーステージ）
		else
			{
			//異常終了
			break;
			}
		}

	//終了
	return(nExitCode);
	}

//戻り値用構造体に最後に送信したデータを書き込み
void CAbh3::SetLastData(pCANABH3_RESULT pDst)
	{
	//保存している送信要素を書き込み
	pDst->DP0S.nOrderAY		= m_var.lastdata.send.nOrderAY;
	pDst->DP0S.nOrderBX		= m_var.lastdata.send.nOrderBX;
	pDst->DP0S.nInputBit	= m_var.lastdata.send.nInputBit;
	}

