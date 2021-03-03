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

//このファイルは、ユーザー側でのみ使用します
//（DLL構築時には不要）

#include "stdafx.h"
#include "CANa31dll.h"

//DLLから関数のエントリを取得
static FARPROC GetDllFunc(HMODULE hModule,char* pFuncName);

//
int32_t GetFunctions(HMODULE hDLL,pCA3_FUNCLIST pFuncList)
	{
	if (hDLL && pFuncList)
		{
		//==========
		//ユーザー用
		//==========

		//使用開始時に呼び出す必要が有ります
		pFuncList->InitInstance			= (CA3_InitInstance)GetDllFunc(hDLL,"InitInstance");
		//使用終了時に呼び出す必要が有ります
		pFuncList->ExitInstance			= (CA3_ExitInstance)GetDllFunc(hDLL,"ExitInstance");
		//使用可能なCANインターフェース数を取得
		pFuncList->GetInterfaceCount	= (CA3_GetInterfaceCount)GetDllFunc(hDLL,"GetInterfaceCount");
		//指定インターフェースを開く
		pFuncList->OpenInterface		= (CA3_OpenInterface)GetDllFunc(hDLL,"OpenInterface");
		//開いたインターフェースを閉じる
		pFuncList->CloseInterface		= (CA3_CloseInterface)GetDllFunc(hDLL,"CloseInterface");
		//現在開いているインターフェース番号を取得
		pFuncList->GetCurrentInterface	= (CA3_GetCurrentInterface)GetDllFunc(hDLL,"GetCurrentInterface");
		//現在インターフェースを開いているか？
		pFuncList->IsOpenInterface		= (CA3_IsOpenInterface)GetDllFunc(hDLL,"IsOpenInterface");
		//インターフェースを開くタイムアウト時間を設定
		pFuncList->SetOpenTimeout		= (CA3_SetOpenTimeout)GetDllFunc(hDLL,"SetOpenTimeout");
		//送信タイムアウト時間を設定
		pFuncList->SetSendTimeout		= (CA3_SetSendTimeout)GetDllFunc(hDLL,"SetSendTimeout");
		//受信タイムアウト時間を設定
		pFuncList->SetRecvTimeout		= (CA3_SetRecvTimeout)GetDllFunc(hDLL,"SetRecvTimeout");
		//通信対象ABH3のアドレスを設定
		pFuncList->SetTargetID			= (CA3_SetTargetID)GetDllFunc(hDLL,"SetTargetID");
		//通信対象ABH3のアドレスを取得
		pFuncList->GetTargetID			= (CA3_GetTargetID)GetDllFunc(hDLL,"GetTargetID");
		//通信ホストのアドレスを設定
		pFuncList->SetHostID			= (CA3_SetHostID)GetDllFunc(hDLL,"SetHostID");
		//通信ホストのアドレスを取得
		pFuncList->GetHostID			= (CA3_GetHostID)GetDllFunc(hDLL,"GetHostID");
		//通信速度を指定
		pFuncList->SetBaudrate			= (CA3_SetBaudrate)GetDllFunc(hDLL,"SetBaudrate");
		//設定した通信速度を取得
		pFuncList->GetBaudrate			= (CA3_GetBaudrate)GetDllFunc(hDLL,"GetBaudrate");
		//PCが起動してからの時間を[ms]単位で取得
		pFuncList->GetTm				= (CA3_GetTm)GetDllFunc(hDLL,"GetTm");
		//指令の初期化
		pFuncList->abh3_can_init		= (CA3_abh3_can_init)GetDllFunc(hDLL,"abh3_can_init");
		//指令の送信（軸別）
		pFuncList->abh3_can_cmdAY		= (CA3_abh3_can_cmdAY)GetDllFunc(hDLL,"abh3_can_cmdAY");
		pFuncList->abh3_can_cmdBX		= (CA3_abh3_can_cmdBX)GetDllFunc(hDLL,"abh3_can_cmdBX");
		//指令の送信（同時）
		pFuncList->abh3_can_cmd			= (CA3_abh3_can_cmd)GetDllFunc(hDLL,"abh3_can_cmd");
		//入力の送信（一括）
		pFuncList->abh3_can_inSet		= (CA3_abh3_can_inSet)GetDllFunc(hDLL,"abh3_can_inSet");
		//入力の送信（ビット）
		pFuncList->abh3_can_inBitSet	= (CA3_abh3_can_inBitSet)GetDllFunc(hDLL,"abh3_can_inBitSet");
		//積算値のリクエスト
		pFuncList->abh3_can_reqPulse	= (CA3_abh3_can_reqPulse)GetDllFunc(hDLL,"abh3_can_reqPulse");
		//ブロードキャストパケットのリクエスト
		pFuncList->abh3_can_reqBRD		= (CA3_abh3_can_reqBRD)GetDllFunc(hDLL,"abh3_can_reqBRD");
		//マルチパケットによるTelABH3パケットの送受信
		pFuncList->abh3_can_trans		= (CA3_abh3_can_trans)GetDllFunc(hDLL,"abh3_can_trans");
		//速度
		pFuncList->cnvVel2CAN			= (CA3_cnvVel2CAN)GetDllFunc(hDLL,"cnvVel2CAN");
		pFuncList->cnvCAN2Vel			= (CA3_cnvCAN2Vel)GetDllFunc(hDLL,"cnvCAN2Vel");
		//電流
		pFuncList->cnvCur2CAN			= (CA3_cnvCur2CAN)GetDllFunc(hDLL,"cnvCur2CAN");
		pFuncList->cnvCAN2Cur			= (CA3_cnvCAN2Cur)GetDllFunc(hDLL,"cnvCAN2Cur");
		//負荷率
		pFuncList->cnvCAN2Load			= (CA3_cnvCAN2Load)GetDllFunc(hDLL,"cnvCAN2Load");
		//アナログ入力
		pFuncList->cnvCAN2Analog		= (CA3_cnvCAN2Analog)GetDllFunc(hDLL,"cnvCAN2Analog");
		//電源電圧
		pFuncList->cnvCAN2Volt			= (CA3_cnvCAN2Volt)GetDllFunc(hDLL,"cnvCAN2Volt");


		//==========
		//デバッグ用
		//==========

		//デバッグ機能の有効無効
		pFuncList->SetDebugMode			= (CA3_SetDebugMode)GetDllFunc(hDLL,"SetDebugMode");
		//デバッグ情報のクリア
		pFuncList->ClearDebugInfo		= (CA3_ClearDebugInfo)GetDllFunc(hDLL,"ClearDebugInfo");
		//デバッグ情報の取得
		pFuncList->GetDebugInfo			= (CA3_GetDebugInfo)GetDllFunc(hDLL,"GetDebugInfo");

		}
	return(0);
	}

//DLLから関数のエントリを取得
FARPROC GetDllFunc(HMODULE hModule,char* pFuncName)
	{
	FARPROC nResult = GetProcAddress(hModule,pFuncName);
	return(nResult);
	}

