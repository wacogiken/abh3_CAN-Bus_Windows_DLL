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

//多重インクルード防止
#ifndef _TYPEDEF_H_INCLUDED_
#define _TYPEDEF_H_INCLUDED_

//プラットフォーム依存
#include <Windows.h>
#include <stdint.h>


//================================================================================
//ユーザーが利用可能な型
//================================================================================

//受信に使用する構造体（受信情報＋受信データ8バイト）
typedef struct _CANABH3_RESULT
	{
	//受信データのCAN-ID
	uint32_t	nID;

	//公開用（送信要素）
	struct _DP0S
		{
		int16_t		nOrderAY;
		int16_t		nOrderBX;
		int32_t		nInputBit;
		} DP0S;

	union
		{
		//内部用
		uint8_t		raw[8];

		//公開用（受信要素）
		struct _DP0R
			{
			int16_t		nBackSpeedA;
			int16_t		nBackSpeedB;
			int16_t		nBackSpeedY;
			int16_t		nBackSpeedX;
			} DP0R;
		struct _DP1R
			{
			int32_t		nInPulseA;
			int32_t		nInPulseB;
			} DP1R;
		struct _BR0
			{
			uint32_t	nErrorBit;
			uint32_t	nWarnBit;
			} BR0;
		struct _BR1
			{
			uint32_t	nCtrlBit;
			uint32_t	nIOflag;
			} BR1;
		struct _BR2
			{
			int16_t		nOrderSpeedAY;
			int16_t		nOrderSpeedBX;
			int16_t		nBackSpeedAY;
			int16_t		nBackSpeedBX;
			} BR2;
		struct _BR3
			{
			int16_t		nOrderCurrentAY;
			int16_t		nOrderCurrentBX;
			int16_t		nLoadA;
			int16_t		nLoadB;
			} BR3;
		struct _BR4
			{
			int32_t		nInPulseA;
			int32_t		nInPulseB;
			} BR4;
		struct _BR5
			{
			int16_t		nAnalog0;
			int16_t		nAnalog1;
			int16_t		nPowerMain;
			int16_t		nPowerCtrl;
			} BR5;
		struct _BR6
			{
			float		nMonitor0;
			float		nMonitor1;
			} BR6;
		struct _BUF
			{
			uint8_t		nData[8];
			} BUF;
		} u;
	} CANABH3_RESULT,*pCANABH3_RESULT;

#endif	//多重インクルード防止
