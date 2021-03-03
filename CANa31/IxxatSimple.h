/*
 ******************************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021, Waco Giken Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the Waco Giken nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************
*/

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
	HANDLE				m_hSema;			//排他制御用セマフォ（関数）
	HANDLE				m_hSema2;			//排他制御用セマフォ（受信バッファ）
	HANDLE				m_hReadThread;		//受信スレッドハンドル
	UINT				m_nThreadID;		//受信スレッドのプロセスID
	bool				m_bQuitThread;		//trueでスレッド停止要求
	can_msg_t*			m_pCanMsg;			//受信バッファ
	uint32_t			m_nCanReadPt;		//受信バッファの読み出し位置
	uint32_t			m_nCanWritePt;

	//非同期受信スレッド
	static unsigned int __stdcall ReceiveThread(void* Param);

	//CAN関数排他制御要求
	bool LockCanMsg(DWORD nTimeoutMS = INFINITE);

	//CNA関数排他制御解除
	void UnlockCanMsg(void);

	//受信バッファ排他制御要求
	bool LockBuffer(void);

	//受信バッファ排他制御解除
	void UnlockBuffer(void);

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
