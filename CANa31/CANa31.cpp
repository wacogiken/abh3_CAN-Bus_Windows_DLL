//概要
//	本DLLでエクスポートする関数を格納しているソースコード

#include "stdafx.h"

//#define _NO_CANA31API_EXPORT	//定義なしでエクスポート、定義有りでインポート
#include "CANa31.h"

//ABH3制御クラスとインターフェースクラスへのポインタ（グローバル）
static CAbh3* g_pAbh3 = NULL;
static CIxxatV2* g_pIF1 = NULL;
static CIxxatSimple* g_pIF2 = NULL;

//DLL使用開始時に呼び出す必要が有ります
CANA31API void InitInstance(int32_t nIFnum)
	{
	//概要
	//	DLL使用開始時に呼び出す必要が有ります
	//パラメータ
	//	nIFnum		使用インターフェース（0...IxxatV2  1...IxxatSimple）
	//戻り値
	//	無し
	//注意点
	//	インターフェースを切り替えたい場合は、終了処理の後、ExitInstanceを
	//	呼び出し、その後でInitInstanceを再度使用してインターフェースを指定します
	//	（接続状態での切り替え不可）

	//
	ExitInstance();
	//
	if(nIFnum == 0)
		{
		g_pIF1 = new CIxxatV2();
		g_pAbh3 = new CAbh3(g_pIF1);
		}
	else if(nIFnum == 1)
		{
		g_pIF2 = new CIxxatSimple();
		g_pAbh3 = new CAbh3(g_pIF2);
		}
	}

//DLL使用終了時に呼び出す必要が有ります
CANA31API void ExitInstance()
	{
	//概要
	//	DLL使用終了時に呼び出す必要が有ります
	//パラメータ
	//	無し
	//戻り値
	//	無し
	//注意点
	//	DLL開放寸前で呼び出して下さい

	if(g_pAbh3)
		{
		delete g_pAbh3;
		g_pAbh3 = NULL;
		}
	if(g_pIF1)
		{
		delete g_pIF1;
		g_pIF1 = NULL;
		}
	if(g_pIF2)
		{
		delete g_pIF2;
		g_pIF2 = NULL;
		}
	}

//使用可能なCANインターフェース数を取得
CANA31API int32_t GetInterfaceCount()
	{
	return(g_pAbh3->GetInterfaceCount());
	}

//指定インターフェースを開く
CANA31API int32_t OpenInterface(int32_t nDeviceNum)
	{
	return(g_pAbh3->OpenInterface(nDeviceNum));
	}

//開いたインターフェースを閉じる
CANA31API void CloseInterface()
	{
	g_pAbh3->CloseInterface();
	}

//現在開いているインターフェース番号を取得
CANA31API int32_t GetCurrentInterface()
	{
	return(g_pAbh3->GetCurrentInterface());
	}

//現在インターフェースを開いているか？
CANA31API int32_t IsOpenInterface()
	{
	return(g_pAbh3->IsOpenInterface());
	}

//インターフェースを開くタイムアウト時間を設定
CANA31API void SetOpenTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetOpenTimeout(nTimeoutMS);
	}

//送信タイムアウト時間を設定
CANA31API void SetSendTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetSendTimeout(nTimeoutMS);
	}

//受信タイムアウト時間を設定
CANA31API void SetRecvTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetRecvTimeout(nTimeoutMS);
	}

//通信対象ABH3のアドレスを設定
CANA31API void SetTargetID(uint8_t nAdrs)
	{
	g_pAbh3->SetTargetID(nAdrs);
	}

//通信対象ABH3のアドレスを取得
CANA31API uint8_t GetTargetID()
	{
	return(g_pAbh3->GetTargetID());
	}

//通信ホストのアドレスを設定
CANA31API void SetHostID(uint8_t nAdrs)
	{
	g_pAbh3->SetHostID(nAdrs);
	}

//通信ホストのアドレスを取得
CANA31API uint8_t GetHostID(void)
	{
	return(g_pAbh3->GetHostID());
	}

//通信速度を指定
CANA31API void SetBaudrate(uint32_t nBaudrateKbps)
	{
	g_pAbh3->SetBaudrate(nBaudrateKbps);
	}

//設定した通信速度を取得
CANA31API uint32_t GetBaudrate()
	{
	return(g_pAbh3->GetBaudrate());
	}

//PCが起動してからの時間を[ms]単位で取得
CANA31API uint32_t GetTm()
	{
	return(g_pAbh3->GetTm());
	}

//指令の初期化
CANA31API int32_t abh3_can_init()
	{
	return(g_pAbh3->abh3_can_init());
	}

//指令の送信（軸別）
CANA31API int32_t abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_cmdAY(cmd,pPtr));
	}
CANA31API int32_t abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_cmdBX(cmd,pPtr));
	}

//指令の送信（同時）
CANA31API int32_t abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_cmd(cmdAY,cmdBX,pPtr));
	}

//入力の送信（一括）
CANA31API int32_t abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_inSet(data,mask,pPtr));
	}

//入力の送信（ビット）
CANA31API int32_t abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_inBitSet(num,data,pPtr));
	}

//積算値のリクエスト
CANA31API int32_t abh3_can_reqPulse(pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_reqPulse(pPtr));
	}

//ブロードキャストパケットのリクエスト
CANA31API int32_t abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_reqBRD(num,pPtr));
	}

//マルチパケットによるTelABH3パケットの送受信
CANA31API int32_t abh3_can_trans(char* sbuf,char* rbuf,size_t& rbuflen)
	{
	//注意点
	//	アプリとDLLのHEAPが同一にならないような使い方（例：MFCアプリで使用）を
	//	した場合、DLL側で確保したバッファをアプリ側で開放出来ない(ASSERTされる)為、
	//	ここでアプリ側のバッファにDLL側のバッファ内容をコピーする
	//
	//パラメータ
	//	sbuf		送信データ(ANSI)
	//	rbuf		受信データ格納領域（アプリ側で確保する）
	//	rbuflen		受信データ格納領域のサイズ（アプリ側で設定する）
	//戻り値
	//	0			正常
	//	-1			異常
	//	-2			バッファサイズが足りない、必要だったサイズはrbuflenに入る

	
	//
	char* pTmp = NULL;
	size_t nTmpLen = 0;
	int32_t nResult = g_pAbh3->abh3_can_trans(sbuf,pTmp,nTmpLen);
	//正常？
	if(nResult == 0)
		{
		//サイズが足りる？
		if(rbuflen >= nTmpLen)
			{
			::CopyMemory(rbuf,pTmp,nTmpLen);
			}
		else
			{
			//足りない
			rbuflen = nTmpLen;	//最低限必要なサイズを設定
			nResult = -2;		//領域サイズが足りない扱い
			}
		//中継している為、開放が必要
		delete pTmp;
		}
	//
	return(nResult);
	}

//速度
CANA31API int16_t	cnvVel2CAN(float vel)
	{
	return(g_pAbh3->cnvVel2CAN(vel));
	}
CANA31API float	cnvCAN2Vel(int16_t vel)
	{
	return(g_pAbh3->cnvCAN2Vel(vel));
	}

//トルク
CANA31API float	cnvCAN2Trq(int16_t trq)
	{
	return(g_pAbh3->cnvCAN2Trq(trq));
	}

//負荷率
CANA31API float	cnvCAN2Load(int16_t load)
	{
	return(g_pAbh3->cnvCAN2Load(load));
	}

//アナログ入力
CANA31API float	cnvCAN2Analog(int16_t analog)
	{
	return(g_pAbh3->cnvCAN2Analog(analog));
	}

//電源電圧
CANA31API float	cnvCAN2Volt(int16_t volt)
	{
	return(g_pAbh3->cnvCAN2Volt(volt));
	}

