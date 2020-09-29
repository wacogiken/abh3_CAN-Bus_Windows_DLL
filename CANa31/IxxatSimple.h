#pragma once

//標準的なインクルード
#include <Windows.h>
#include <stdint.h>

//継承元クラス
#include "CanIF.h"

//本クラスで使用するデバイス用のインクルード
#include "simply.h"

//本クラス定義
class CIxxatSimple : public CCanIF
{
public:
//================================================================================
//使用CANインターフェース毎に実装が必須の仮想関数
//================================================================================
public:

	//利用可能なCANインターフェース数を取得する場合に呼び出されます
	virtual int32_t OnGetInterfaceCount(void);

	//指定CANインターフェースを開く場合に呼び出されます
	virtual int32_t OnOpenInterface(int32_t nDeviceNum);

	//開いたCANインターフェースを閉じる場合に呼び出されます
	virtual void OnCloseInterface(void);

	//CANインターフェースから受信する場合に呼び出されます
	virtual int32_t OnCanRecv(uint32_t* pCanID,uint8_t* pData8);

	//CANインターフェースに送信する場合に呼び出されます
	virtual int32_t OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength);

	//CANインターフェースにエラーが有るか調べる場合に呼び出されます
	virtual int32_t OnCanGetError(void);

	//CANインターフェースのエラーを解除する場合に呼び出されます
	virtual int32_t OnCanClearError(void);

	//受信バッファをクリアする場合に呼び出されます
	virtual int32_t OnCanRecvClear(void);
	
//================================================================================
//内部用（スレッドから利用為にpublic扱い）
//================================================================================
public:
	//デバイス固有のステータス状態
	typedef struct _IXXATSMPL_STATUS
		{
		bool	bOpen;		//false(回線を開いてない) true(回線を開いている）
		union
			{
			uint16_t nRaw;
			struct _errbit_info
				{
				uint16_t	nRunning:1;
				uint16_t	nReset:1;
				uint16_t	nBusoff:1;
				uint16_t	nErrorStatus:1;
				uint16_t	nTXoverrun:1;
				uint16_t	nRXoverrun:1;
				uint16_t	nPending:1;
				uint16_t	nDummy:9;
				} info;
			} err;
		} IXXATSMPL_STATUS,*pIXXATSMPL_STATUS;
	//
	IXXATSMPL_STATUS	m_status;
	HANDLE				m_hSema;			//受信バッファ排他制御用セマフォ
	HANDLE				m_hReadThread;		//受信スレッドハンドル
	UINT				m_nThreadID;		//受信スレッドのプロセスID
	bool				m_bQuitThread;		//trueでスレッド停止要求
	can_msg_t*			m_pCanMsg;			//受信バッファ
	uint32_t			m_nCanReadPt;		//受信バッファの読み出し位置
	uint32_t			m_nCanWritePt;

	//非同期受信スレッド
	static unsigned int __stdcall ReceiveThread(void* Param);

	//受信バッファ排他制御要求
	bool LockCanMsg(DWORD nTimeoutMS = INFINITE);

	//受信バッファ排他制御解除
	void UnlockCanMsg(void);

	//受信データの登録処理
	uint32_t AddCanMsg(can_msg_t* pMsg,int nCount = 1);

	//登録されたCANメッセージを1つ取得
	uint32_t GetCanMsg(can_msg_t* pMsg);

//================================================================================
//内部用
//================================================================================
protected:
	//
	void debugprint(bool bSend,uint32_t nCanID,uint8_t* pData)
		{
		//printf("%8d %08X ",GetTm(),nCanID);
		//if(!bSend)
		//	printf("\t");
		//for(int32_t nLoop = 0;nLoop < 8;nLoop++)
		//	printf("%02X ",pData[nLoop]);
		//printf("\n");
		}

public:
	CIxxatSimple();
	virtual ~CIxxatSimple();
};
