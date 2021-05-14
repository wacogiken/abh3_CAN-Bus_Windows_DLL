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

#include <Windows.h>
#include <stdint.h>
#include "CANa31dll.h"

//============================================================
//エクスポート？
//============================================================

 #ifndef _NO_CANA31API_EXPORT
  #define CANA31API __declspec(dllexport)
#else
  #define CANA31API __declspec(dllimport)
#endif

//============================================================
//以下リンケージはC言語形式とする
//============================================================

#ifdef __cplusplus
 extern "C" {
#endif //__cplusplus

//============================================================
//ユーザー用関数
//============================================================

//使用開始時に呼び出す必要が有ります
CANA31API void InitInstance(int32_t nIFnum);

//使用終了時に呼び出す必要が有ります
CANA31API void ExitInstance(void);

//使用可能なCANインターフェース数を取得
CANA31API int32_t GetInterfaceCount(void);

//指定インターフェースを開く
CANA31API int32_t OpenInterface(int32_t nDeviceNum);

//開いたインターフェースを閉じる
CANA31API void CloseInterface(void);

//現在開いているインターフェース番号を取得
CANA31API int32_t GetCurrentInterface(void);

//現在インターフェースを開いているか？
CANA31API int32_t IsOpenInterface(void);

//インターフェースを開くタイムアウト時間を設定
CANA31API void SetOpenTimeout(uint32_t nTimeoutMS);

//送信タイムアウト時間を設定
CANA31API void SetSendTimeout(uint32_t nTimeoutMS);

//受信タイムアウト時間を設定
CANA31API void SetRecvTimeout(uint32_t nTimeoutMS);

//通信対象ABH3のアドレスを設定
CANA31API void SetTargetID(uint8_t nAdrs);

//通信対象ABH3のアドレスを取得
CANA31API uint8_t GetTargetID(void);

//通信ホストのアドレスを設定
CANA31API void SetHostID(uint8_t nAdrs);

//通信ホストのアドレスを取得
CANA31API uint8_t GetHostID(void);

//通信速度を指定
CANA31API void SetBaudrate(uint32_t nBaudrateKbps);

//設定した通信速度を取得
CANA31API uint32_t GetBaudrate(void);

//PCが起動してからの時間を[ms]単位で取得
CANA31API uint32_t GetTm(void);

//指令の初期化
CANA31API int32_t abh3_can_init(void);

//指令の送信（軸別）
CANA31API int32_t abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr);
CANA31API int32_t abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr);

//指令の送信（同時）
CANA31API int32_t abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr);

//入力の送信（一括）
CANA31API int32_t abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr);

//入力の送信（ビット）
CANA31API int32_t abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr);

//積算値のリクエスト
//2021.05.14 機能無効化の為、削除
//CANA31API int32_t abh3_can_reqPulse(pCANABH3_RESULT pPtr);

//ブロードキャストパケットのリクエスト
CANA31API int32_t abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr);

//マルチパケットによるTelABH3パケットの送受信
CANA31API int32_t abh3_can_trans(char* sbuf,char* rbuf,uint32_t* rbuflen);

//速度から内部値へ変換
CANA31API int16_t cnvVel2CAN(float vel);

//内部値から速度へ変換
CANA31API float	cnvCAN2Vel(int16_t vel);

//電流値から内部値に変換
CANA31API int16_t cnvCur2CAN(float cur);

//内部値から電流値に変換
CANA31API float	cnvCAN2Cur(int16_t cur);

//内部値から負荷率に変換
CANA31API float	cnvCAN2Load(int16_t load);

//内部値からアナログ入力に変換
CANA31API float	cnvCAN2Analog(int16_t analog);

//内部値から電源電圧に変換
CANA31API float	cnvCAN2Volt(int16_t volt);



//============================================================
//デバッグ用
//============================================================

//デバッグ機能の有効無効設定
CANA31API int32_t SetDebugMode(int32_t nEnable);

//デバッグ情報のクリア
CANA31API int32_t ClearDebugInfo(uint8_t nDummy);

//デバッグ情報の取得
CANA31API int32_t GetDebugInfo(int32_t nPt,uint8_t* pMemory9);


//============================================================
//リンケージ解除
//============================================================
#ifdef __cplusplus
 }
#endif //__cplusplus
