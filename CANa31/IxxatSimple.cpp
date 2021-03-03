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
//========================================
//概要
//========================================
//	Ixxat Simpleのデバイスを使う為のクラス
//
//========================================
//履歴
//========================================
//	2020.09.xx	yo0043	1st release
//	2020.10.01	yo0043	2nd release
//						simply_open時、既存COMポート(not CAN)を開いた場合に
//						一度simply_closeを呼び出さないと開けなくなる問題に対処
//
//========================================
//実装に必要な前処理
//========================================
//	IxxatSimple用のドライバをダウンロードして展開（SDKも入っている）
//	プロジェクト設定で、追加インクルードフォルダに以下を追加
//		"<展開したフォルダ>\inc"
//	プロジェクト設定で、追加ライブラリフォルダに以下を追加
//		"<展開したフォルダ>\lib\dll"
//	プロジェクトの実行ファイルが格納される場所に、上記ライブラリフォルダ内のDLLをコピー
//
//========================================
//使用方法
//========================================
//	基本的な使い方は、サンプルソースを見て下さい
//
//========================================
//デバイス固有
//========================================
//エラー発生時
//	基本的に関数でエラーが発生したら、回線を切断し、接続する所から
//	やり直しを推奨します。（リセットで全要素が消えるので、結局繋ぎ直し処理必須となる為）
//
//ボーレートには以下の値のみが設定可能です
//		値		設定			値		設定	
//	---------------------------------------------			
//		10	->	10Kbps			20	->	20Kbps
//		50	->	50Kbps			100	->	100Kbps
//		125	->	125Kbps			250	->	250Kbps
//		500	->	500Kbps			800	->	800Kbps
//		1000->	1000Kbps
//	---------------------------------------------			
//
//OnGetInterfaceCountの戻り値
//	常に1が戻ります
//
//OnOpenInterfaceに渡すデバイスの番号
//	COMポート番号(1開始)を渡して下さい
//
//SetTimeout_Open,SetTimeout関数で指定する回線を開く時のタイムアウト値
//	設定は無視されます
//
//SetTimeout_Send,SetTimeout関数で指定する送信時のタイムアウト値
//	設定は無視されます
//
//OnCanGetErrorで戻るエラーステータスは、CAN受信を行った時にエラーに
//ならない限り、値が更新されません（非同期で更新されない）
//
//========================================

//コンソールへのデバッグ出力（送信及び受信）0以外で有効
#define	DEBUGPRINT	(0)

//
#include "stdafx.h"
#include "IxxatSimple.h"
#include <process.h>
#include <stdio.h>

//本クラスで扱うデバイスに必須のライブラリ
#pragma comment(lib,"simplyCAN.lib")

//安全な解放
template <class T> void SafeRelease(T **ppT)
	{
	if (*ppT)
		{
		(*ppT)->Release();
		*ppT = NULL;
		}
	}

//安全な削除
template <class T> void SafeDelete(T **ppT)
	{
	if (*ppT)
		{
		delete *ppT;
		*ppT = NULL;
		}
	}

//コンストラクタ
CIxxatSimple::CIxxatSimple()
	{
	//ステータス保存用領域を初期化
	memset(&m_status,0,sizeof(IXXATSMPL_STATUS));
	//その他初期化
	m_hReadThread = NULL;
	m_bQuitThread = false;
	m_nCanReadPt = 0;
	m_nCanWritePt = 0;
	m_pCanMsg = NULL;

	//排他制御用セマフォ
	m_hSema = ::CreateSemaphore(NULL,1,1,NULL);
	m_hSema2 = ::CreateSemaphore(NULL,1,1,NULL);
	}

//デストラクタ
CIxxatSimple::~CIxxatSimple()
	{
	//回線を開いている場合が有るので閉じる処理を行う
	OnCloseInterface();

	//排他制御用セマフォ開放
	if(m_hSema)
		{
		::CloseHandle(m_hSema);
		m_hSema = NULL;
		}
	if(m_hSema2)
		{
		::CloseHandle(m_hSema2);
		m_hSema2 = NULL;
		}

	}

//================================================================================
//使用CANインターフェース毎に実装が必須の仮想関数
//================================================================================

//利用可能なCANインターフェース数を取得する場合に呼び出されます
int32_t CIxxatSimple::OnGetInterfaceCount()
	{
	//概要
	//	指定CANインターフェースの利用可能数を取得する場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	0			利用可能インターフェースがありません
	//	-1			調査時にエラーが発生しました
	//	上記以外	利用可能なCANインターフェース数
	//

	//todo:CANインターフェースの数を戻す処理を実装して下さい

	//完了
	return(1);
	}

//指定CANインターフェースを開く場合に呼び出されます
int32_t CIxxatSimple::OnOpenInterface(int32_t nDeviceNum)
	{
	//概要
	//	指定CANインターフェースを開く場合に呼び出されます
	//パラメータ
	//	nDeviceNum		1開始のCOMポート番号
	//戻り値
	//	0				正常終了
	//	0以外			異常終了(simply_get_last_errorの戻り値参照)

	//todo:CANインターフェースを開く処理を実装して下さい

	int nResult = -1;

	//開いている場合が有るので閉じる
	OnCloseInterface();

	//排他制御開始
	if(!LockCanMsg())
		return(-1);	//資源占有失敗

	//
	do
		{
		//デバイス名を構築して回線を開く
		char* pDeviceName = new char[16]();
		sprintf_s(pDeviceName,16,"COM%d",nDeviceNum);
		bool bResult = simply_open(pDeviceName);
		delete pDeviceName;
		if(!bResult)
			{
			//失敗
			simply_close();		//既存COMポートを間違って開いた場合、内部的に開いたままになる模様で
								//明示的に閉じないと次回のsimply_openで占有されている扱いとなる
			//内部エラーコードを戻す
			nResult = (int32_t)simply_get_last_error();
			break;
			}

		//回線の初期化
		if(!simply_initialize_can(uint16_t(GetBaudrate())))
			{
			//失敗
			simply_close();
			//内部エラーコードを戻す
			nResult = (int32_t)simply_get_last_error();
			break;
			}

		//CANの開始
		if(!simply_start_can())
			{
			//失敗
			simply_reset_can();
			simply_close();
			//内部エラーコードを戻す
			nResult = (int32_t)simply_get_last_error();
			break;
			}

		//ステータスの初期化
		m_status.err.nRaw = 0;

		//受信データ格納用領域の初期化
		m_pCanMsg = new can_msg_t[256]();
		m_nCanReadPt = 0;
		m_nCanWritePt = 0;

		//受信スレッド開始
		m_bQuitThread = false;
		::SetThreadPriority(m_hReadThread = (HANDLE)_beginthreadex(NULL,0,&CIxxatSimple::ReceiveThread,this,NULL,&m_nThreadID),THREAD_PRIORITY_NORMAL);

		//接続済みフラグ設定
		m_status.bOpen = true;

		//
		nResult = 0;
		break;
		} while(1);

	//排他制御開放
	UnlockCanMsg();

	//処理完了
	return(nResult);
	}

//開いたCANインターフェースを閉じる場合に呼び出されます
void CIxxatSimple::OnCloseInterface()
	{
	//概要
	//	開いたCANインターフェースを閉じる場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	無し

	//todo:CANインターフェースを開いている場合に閉じる処理を実装して下さい

	//排他制御
	if(!LockCanMsg())
		return;	//資源占有失敗

	//受信スレッド動作中？
	if (m_hReadThread)
		{
		//受信スレッド停止
		m_bQuitThread = true;
		::WaitForSingleObject(m_hReadThread,INFINITE);
		::CloseHandle(m_hReadThread);
		m_hReadThread = NULL;
		}

	//回線を開いている？
	if(m_status.bOpen)
		{
		simply_stop_can();
		simply_reset_can();
		simply_close();
		m_status.bOpen = false;
		}

	//受信バッファ開放
	SafeDelete(&m_pCanMsg);

	//排他制御開放
	UnlockCanMsg();
	}

//CANインターフェースから受信する場合に呼び出されます
int32_t CIxxatSimple::OnCanRecv(uint32_t* pCanID,uint8_t* pData8)
	{
	//概要
	//	CANインターフェースから1つ分のパケットを受信します
	//パラメータ
	//	pCanID			受信パケットのIDを格納する領域へのポインタ
	//	pData8			受信パケットを格納する8[bytes]領域へのポインタ
	//	nTimeoutMS		受信許容時間[ms]
	//戻り値
	//	0				正常終了
	//	0以外			タイムアウト

	//todo:CANインターフェースから1回分の受信を行う処理を実装して下さい

	//注意点
	//	GetCanMsg関数がバッファの占有を行っている為、本関数内で
	//	バッファの占有を行ってはならない

	//戻り値
	int32_t nResult = -1;

	//受信タイムアウト絶対時間を算出
	uint32_t nLimitTime = GetTm() + GetTimeout_Recv();

	//制限時間内のみ処理する（但し最低1回は処理する）
	do
		{
		can_msg_t msg;
		if (GetCanMsg(&msg) == 0)
			{
			//正常取得
			*pCanID = uint32_t(msg.ident);
			::memcpy(pData8,msg.payload,sizeof(char) * 8);
			nResult = 0;
			break;
			}
		else
			{
			Sleep(1);
			}
		} while(GetTm() < nLimitTime);

	//
	return(nResult);
	}

//CANインターフェースに送信する場合に呼び出されます
int32_t CIxxatSimple::OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
	{
	//概要
	//	CANインターフェースに1つ分のパケットを送信します
	//パラメータ
	//	nCanID			送信パケットのID
	//	pData8			送信パケットが格納されている8[bytes]領域へのポインタ
	//	nTimeoutMS		送信許容時間[ms]
	//戻り値
	//	0				正常終了
	//	0以外			異常終了(simply_get_last_errorの戻り値参照)
	//注意点
	//	IxxatSimpleの方は、送信前にステータスを確認し、PENDING状態になっていない事を
	//	確認してから送信する必要が有る



	//todo:CANインターフェースに送信する処理を実装して下さい

	//戻り値
	int32_t nResult = -1;

	//排他制御開始
	if(!LockCanMsg())
		return(-1);	//資源占有失敗

	do
		{
		//For debug
		if(DEBUGPRINT)
			debugprint(true,nCanID,pData8);

		//ステータス取得して送信可能になる迄待つ
		can_sts_t status;
		while(-1)
			{
			if(!simply_can_status(&status))
				{
				nResult = (int32_t)simply_get_last_error();
				break;
				}
			else if(status.sts & CAN_STATUS_PENDING)
				Sleep(1);
			else
				{
				nResult = 0;
				break;	//送信可能
				}
			}
		if(nResult)
			break;

		//送信
		can_msg_t msg;
		msg.dlc = nLength;
		msg.ident = nCanID | 0x80000000;
		memcpy(msg.payload,pData8,sizeof(char) * 8);
		if(!simply_send(&msg))
			{
			//送信失敗
			nResult = (int32_t)simply_get_last_error();
			break;
			}
		//
		nResult = 0;
		break;
		} while(-1);

	//
	UnlockCanMsg();
	//
	return(nResult);
	}

//CANインターフェースにエラーが有るか調べる場合に呼び出されます
int32_t CIxxatSimple::OnCanGetError()
	{
	//概要
	//	CANインターフェースにエラーが有るか調べます
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常
	//注意点
	//	このデバイスでは発生したエラーが全て受信データとして通知される

	//todo:CANインターフェースにエラーが有るか調べる処理を実装して下さい

	//内部に格納したエラーが有るか？
	if(m_status.err.info.nErrorStatus)
		return(1);

	return(0);
	}

//CANインターフェースのエラーを解除する場合に呼び出されます
int32_t CIxxatSimple::OnCanClearError()
	{
	//概要
	//	CANインターフェースのエラーを解除します
	//	内部保存のエラーコードを解除するだけで、ハードウェアのエラーは解除不可
	//	（ハードウェアエラーは、一度完全に閉じないと(simply_close)解除出来ない）
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常

	//todo:CANインターフェースのエラーを解除する処理を実装して下さい

	//内部に格納したエラー値を解除
	m_status.err.nRaw = 0;

	return(0);
	}

//受信バッファをクリアする場合に呼び出されます
int32_t CIxxatSimple::OnCanRecvClear()
	{
	//概要
	//	受信バッファをクリアする場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常
	//注意点
	//	既に資源占有された状態で呼び出されるので、資源占有しない事


	//todo:受信バッファをクリアする処理を実装して下さい

	//受信バッファ排他制御要求
	if(LockBuffer())
		{
		//受信バッファ位置を初期化
		m_nCanReadPt = 0;
		m_nCanWritePt = 0;
		//受信バッファ排他制御解除
		UnlockBuffer();
		}
	//
	return(0);
	}

//================================================================================
//本クラス専用の要素
//================================================================================

//非同期受信スレッド
unsigned int __stdcall CIxxatSimple::ReceiveThread(void* Param)
	{
	//動作概要
	//	受信した場合
	//		有効なデータの場合
	//			受信データを登録し、待ち時間なしで再度受信に戻る
	//		無効なデータの場合
	//			待ち時間なしで再度受信に戻る
	//	受信しなかった、又はエラーが出た場合
	//		一定時間待って、受信に戻る

	//呼び出し元クラスへのポインタ
	CIxxatSimple* pClass = (CIxxatSimple*)Param;

	//
	DWORD		nWaitTimeoutMS = 10;	//10[ms]
	uint32_t	nStage = 0;				//受信待ちステージから開始
	can_msg_t	canMsg;

	//
	while (!pClass->m_bQuitThread)
		{
		//受信ステージ
		if (nStage == 0)
			{
			//排他制御
			if(pClass->LockCanMsg())
				{
				//受信
				int8_t nResult = simply_receive(&canMsg);
				//メッセージ受信？
				if(nResult == 1)
					{
					//特殊メッセージ？
					if(canMsg.dlc & 0x80)
						{
						//登録しない
						}
					else
						{
						//IDに細工
						canMsg.ident &= 0x7fffffff;
						//登録
						pClass->AddCanMsg(&canMsg,1);
						}
					}
				//エラー又は受信無し？
				else
					nStage = 1; 
				//排他制御開放
				pClass->UnlockCanMsg();
				}
			else
				{
				//排他制御失敗は時間待ちに遷移
				nStage = 1;
				}
			}
		//時間待ちステージ
		else if (nStage == 1)
			{
			//一定時間待つ
			Sleep(1);
			//受信ステージに遷移
			nStage = 0;
			}
		}
	//
	_endthreadex(0);
	return(0);
	}

//排他制御要求
bool CIxxatSimple::LockCanMsg(DWORD nTimeoutMS /* INFINITE */)
	{
	//占有開始
	if(::WaitForSingleObject(m_hSema,nTimeoutMS) != WAIT_OBJECT_0)
		return(false);	//確保失敗
	//確保成功
	return(true);
	}

//排他制御解除
void CIxxatSimple::UnlockCanMsg()
	{
	//占有開放
	::ReleaseSemaphore(m_hSema,1,NULL);
	//他のスレッドに制御を渡す（待っているスレッドが有れば）
	Sleep(0);
	}

//受信バッファ排他制御要求
bool CIxxatSimple::LockBuffer()
	{
	//占有開始
	if(::WaitForSingleObject(m_hSema2,INFINITE) != WAIT_OBJECT_0)
		return(false);	//確保失敗
	//確保成功
	return(true);
	}

//受信バッファ排他制御解除
void CIxxatSimple::UnlockBuffer()
	{
	//
	//占有開放
	::ReleaseSemaphore(m_hSema2,1,NULL);
	//他のスレッドに制御を渡す（待っているスレッドが有れば）
	Sleep(0);
	}



//受信データの登録処理
uint32_t CIxxatSimple::AddCanMsg(can_msg_t* pMsg,int nCount /* 1 */)
	{
	//注意点
	//	この関数内で使用している排他制御は、バッファの方なので注意

	//受信バッファ排他制御要求
	if(!LockBuffer())
		return(-1);

	uint32_t nResult = 0;
	while(nCount)
		{
		//For DEBUG
		if(DEBUGPRINT)
			debugprint(false,pMsg->ident,pMsg->payload);

		//このパケットは無視しない対象か？
		if(!IsIgnoreID(pMsg->ident))
			{
			//バッファFULLでは無い？
			if(m_nCanWritePt < 255)
				{
				//格納
				::memcpy(&m_pCanMsg[m_nCanWritePt],pMsg,sizeof(can_msg_t));
				//格納先更新
				++m_nCanWritePt;
				//格納数更新
				++nResult;
				}
			else
				{
				//バッファが一杯
				}
			}
		else
			{
			//無視する対象
			}

		//ソースの次位置
		++pMsg;

		//残り更新
		--nCount;
		}

	//受信バッファ排他制御解除
	UnlockBuffer();

	//
	return(nResult);
	}

//登録されたCANメッセージを1つ取得
uint32_t CIxxatSimple::GetCanMsg(can_msg_t* pMsg)
	{
	//受信バッファ排他制御要求
	if(!LockBuffer())
		return(2);	//資源占有失敗時は、その他エラーとして戻る

	//
	uint32_t nResult = 0;

	//未読み出しの要素が有る？
	if(m_nCanReadPt < m_nCanWritePt)
		{
		//該当箇所から取得
		::memcpy(pMsg,&m_pCanMsg[m_nCanReadPt],sizeof(can_msg_t));
		//読み出し位置の更新
		++m_nCanReadPt;
		//読み出し残りが無い？
		if(m_nCanReadPt >= m_nCanWritePt)
			{
			//初期化（全クリア扱いでバッファを先頭に戻すが、排他制御済みなのでOnCanRecvClearを使わない事）
			m_nCanReadPt = 0;
			m_nCanWritePt = 0;
			}
		nResult = 0;
		}
	else
		nResult = 1;

	//受信バッファ排他制御解除
	UnlockBuffer();
	//
	return(nResult);
	}
