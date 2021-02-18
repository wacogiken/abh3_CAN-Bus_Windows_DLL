//多重インクルード防止
#ifndef _ABH3_H_INCLUDED_
#define _ABH3_H_INCLUDED_

//プラットフォーム依存
#include <Windows.h>
#include <stdint.h>
#include <timeapi.h>
#include <stdio.h>

//CAN関連
#include "Can1939.h"
#include "CanIF.h"

//型定義
#include "typedef.h"

//クラス定義
class CAbh3
{
//================================================================================
//ユーザーが利用可能な型
//================================================================================
public:

//================================================================================
//内部で使用される型
//================================================================================
protected:

//================================================================================
//ユーザーが利用可能な関数（基本要素）
//================================================================================
public:
	//使用可能なCANインターフェース数を取得
	int32_t GetInterfaceCount(void);

	//指定インターフェースを開く
	int32_t OpenInterface(int32_t nDeviceNum);

	//開いたインターフェースを閉じる
	void CloseInterface(void);

	//現在開いているインターフェース番号を取得
	int32_t GetCurrentInterface(void);

	//現在インターフェースを開いているか？
	int32_t IsOpenInterface(void);

	//インターフェースを開くタイムアウト時間を設定
	void SetOpenTimeout(uint32_t nTimeoutMS);

	//送信タイムアウト時間を設定
	void SetSendTimeout(uint32_t nTimeoutMS);

	//受信タイムアウト時間を設定
	void SetRecvTimeout(uint32_t nTimeoutMS);

	//通信対象ABH3のアドレスを設定
	void SetTargetID(uint8_t nAdrs);

	//通信対象ABH3のアドレスを取得
	uint8_t GetTargetID(void);

	//通信ホストのアドレスを設定
	void SetHostID(uint8_t nAdrs);

	//通信ホストのアドレスを取得
	uint8_t GetHostID(void);

	//通信速度を指定
	void SetBaudrate(uint32_t nBaudrateKbps);

	//設定した通信速度を取得
	uint32_t GetBaudrate(void);

	//PCが起動してからの時間を[ms]単位で取得
	uint32_t GetTm(void)
		{
		uint32_t nResult = 0;
		if(!IsValidDevice())
			nResult = m_var.pDeviceClass->GetTm();
		return(nResult);
		}	

//================================================================================
//ユーザーが利用可能な関数（ABH3要素）
//================================================================================
public:
	//指令の初期化
	int32_t abh3_can_init(void);

	//指令の送信（軸別）
	int32_t abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr);
	int32_t abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr);

	//指令の送信（同時）
	int32_t abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr);

	//入力の送信（一括）
	int32_t abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr);

	//入力の送信（ビット）
	int32_t abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr);

	//積算値のリクエスト
	int32_t abh3_can_reqPulse(pCANABH3_RESULT pPtr);

	//ブロードキャストパケットのリクエスト
	int32_t abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr);

	//マルチパケットによるTelABH3パケットの送受信
	int32_t abh3_can_trans(char* sbuf,char*& rbuf,size_t& rbuflen);

//================================================================================
//ユーザーが利用可能な関数（変換用）
//================================================================================
public:
	//速度
	int16_t	cnvVel2CAN(float vel)
		{
		int16_t nResult = int16_t(vel / 0.2f);
		return(nResult);
		}
	float	cnvCAN2Vel(int16_t vel)
		{
		float nResult = float(vel) * 0.2f;
		return(nResult);
		}

	//トルク
	float	cnvCAN2Trq(int16_t trq)
		{
		float nResult = float(trq) * 0.01f;
		return(nResult);
		}

	//負荷率
	float	cnvCAN2Load(int16_t load)
		{
		float nResult = float(load) * 1.0f;
		return(nResult);
		}

	//アナログ入力
	float	cnvCAN2Analog(int16_t analog)
		{
		float nResult = float(analog) * 0.01f;
		return(nResult);
		}

	//電源電圧
	float	cnvCAN2Volt(int16_t volt)
		{
		float nResult = float(volt) * 0.1f;
		return(nResult);
		}

//================================================================================
//内部用
//================================================================================
protected:

	//デバイスクラスが登録されているか？
	bool IsValidDevice(void)
		{
		//登録されていればtrueが戻る
		return((bool)(m_var.pDeviceClass != 0));
		}

	//シングルパケットのIDを構築
	uint32_t CreateID(uint32_t nBaseID)
		{
		//下位16bitに送信先と送信元のIDが入る
		nBaseID &= 0xffff0000;
		nBaseID |= (m_var.config.nTargetAdrs << 8);
		nBaseID |= m_var.config.nHostAdrs;
		return(nBaseID);
		}

	//ブロードキャストのIDを構築
	uint32_t CreateBrdID(uint8_t nTarget)
		{
		//下位8bitにターゲットの番号が入る
		uint32_t nResult = 0x00ff00 | nTarget;
		return(nResult);
		}

	//排他制御要求
	int32_t Lock(void)
		{
		//
		if(!IsValidDevice())
			return(-1);	//デバイスクラスが登録されていない

		//送信タイムアウト時間を取得
		uint32_t nTimeoutMS = m_var.pDeviceClass->GetTimeout_Send();

		//ロックする時は送信許容時間を利用する
		if(::WaitForSingleObject(m_var.hTerm,nTimeoutMS) != WAIT_OBJECT_0)
			return(-1);	//資源占有失敗
		//資源占有成功
		return(0);
		}

	//排他制御開放
	void Unlock(void)
		{
		//注意点
		//	確認しないで開放する為、2重開放の類に注意
		::ReleaseSemaphore(m_var.hTerm,1,NULL);
		}

	//受信バッファクリア
	void ClearRecvBuffer(void)
		{
		if(IsValidDevice())
			m_var.pDeviceClass->OnCanRecvClear();
		}

	//CAN送信
	int32_t CanSend8(uint32_t nSendID,uint8_t* pSendData,uint8_t nLength);

	//CAN受信
	int32_t CanRecv8(uint32_t* pRecvID,uint8_t* pRecvData); 

	//シングルパケット送受信
	int32_t CanTermSingle(uint32_t nSendID,uint8_t* pSendData,pCANABH3_RESULT pResult,uint8_t nLength);
	int32_t CanTermSingle(uint32_t nSendID,uint32_t nOnlyID,uint8_t* pSendData,pCANABH3_RESULT pResult,uint8_t nLength);

	//マルチパケット送信
	int32_t CanTermSendMulti(uint8_t* pSendData,uint32_t nSendDataSize,uint8_t*& pRecvData,uint32_t& nRecvDataSize);

	//戻り値用構造体に最後に送信したデータを書き込み
	void SetLastData(pCANABH3_RESULT pDst);


	//内部用変数
	typedef struct _CANABH3_VAR
		{
		CCanIF*				pDeviceClass;	//出力先のクラス
		int32_t				nOpenDevice;	//0..開いていない  1以上..開いたインターフェース番号+1が入る
		HANDLE				hTerm;			//通信排他用のセマフォ
		//
		struct _CANABH3_CONFIG
			{
			uint8_t			nTargetAdrs;	//ABH3側アドレス
			uint8_t			nHostAdrs;		//ホスト側（PC）アドレス
			} config;
		//
		struct _CANABH3_LASTDATA
			{
			//過去に把握したデータ（送信）
			struct _CANABH3_LASTDATA_SEND
				{
				uint32_t	nCanID;
				int16_t		nOrderAY;
				int16_t		nOrderBX;
				uint32_t	nInputBit;
				} send;
			//過去に把握したデータ（受信）
			struct _CANABH3_LASTDATA_RECV
				{
				//uint32_t	nCanID;
				int16_t		nBackSpeedA;
				int16_t		nBackSpeedB;
				int16_t		nBackSpeedY;
				int16_t		nBackSpeedX;
				int32_t		nInPulseA;
				int32_t		nInPulseB;
				uint32_t	nErrorBit;
				uint32_t	nWarnBit;
				uint32_t	nCtrlBit;
				uint32_t	nIOflag;
				int16_t		nOrderSpeedAY;
				int16_t		nOrderSpeedBX;
				int16_t		nBackSpeedAY;
				int16_t		nBackSpeedBX;
				int16_t		nOrderCurrentAY;
				int16_t		nOrderCurrentBX;
				int16_t		nLoadA;
				int16_t		nLoadB;
				int16_t		nAnalog0;
				int16_t		nAnalog1;
				int16_t		nPowerMain;
				int16_t		nPowerCtrl;
				float		nMonitor0;
				float		nMonitor1;
				} recv;

			} lastdata;

		//デバッグ用
		struct _CANABH3_DEBUG
			{
			bool			bEnable;	//デバッグ機能の有効無効設定
			int				nInfoPt;	//次の書き込み場所
			uint8_t*		pInfoData;	//9バイト単位で使う(先頭が0で送信、1で受信)
			} debug;

		} CANABH3_VAR,*pCANABH3_VAR;
	CANABH3_VAR m_var;

protected:

	//デバッグ情報の追加
	void AddDebugInfo(bool bSend,uint8_t* pData8)
		{
		//デバッグ機能が有効か？
		if(m_var.debug.bEnable)
			{
//
int x = 0;
for(int nPt = 0;nPt < 8;nPt++)
	x += pData8[nPt];
if(x == 0)
Sleep(0);








			//初回利用？
			if(m_var.debug.pInfoData == NULL)
				{
				m_var.debug.pInfoData = new uint8_t[512 * 9]();
				m_var.debug.nInfoPt = 0;
				}

			//書き込めるか？
			if(m_var.debug.nInfoPt < 512)
				{
				//配列番号
				int nPt = 9 * m_var.debug.nInfoPt;
				//
				if(bSend)
					m_var.debug.pInfoData[nPt + 0] = 0;
				else
					m_var.debug.pInfoData[nPt + 0] = 1;
				memcpy(&m_var.debug.pInfoData[nPt + 1],pData8,8);
				//
				++m_var.debug.nInfoPt;
				}
			}
		}

public:
	//デバッグ機能の有効無効
	void SetDebugMode(bool bEnable = false)
		{
		if(!bEnable)
			ClearDebugInfo();
		m_var.debug.bEnable = bEnable;
		}

	//デバッグ情報の初期化
	void ClearDebugInfo(void)
		{
		m_var.debug.nInfoPt = 0;
		if(m_var.debug.pInfoData)
			{
			delete m_var.debug.pInfoData;
			m_var.debug.pInfoData = NULL;
			}
		}

	//デバッグ情報の取得
	uint8_t* GetDebugInfo(int nPt)
		{
		if(nPt < m_var.debug.nInfoPt)
			return(&m_var.debug.pInfoData[nPt * 9]);
		return(NULL);
		}

public:
	//コンストラクタ
	CAbh3(CCanIF* pDeviceClass = NULL);
	//デストラクタ
	virtual ~CAbh3();
};

#endif
