//多重インクルード防止
#ifndef _TYPEDEF_H_INCLUDED_
#define _TYPEDEF_H_INCLUDED_

//プラットフォーム依存
#include <Windows.h>
#include <stdint.h>


//================================================================================
//ユーザーが利用可能な型
//================================================================================

//受信に使用する構造体
typedef struct _CANABH3_RESULT
	{
	uint32_t	nID;
	union
		{
		uint8_t		raw[8];
		struct _r0
			{
			int16_t		nBackSpeedA;
			int16_t		nBackSpeedB;
			int16_t		nBackSpeedY;
			int16_t		nBackSpeedX;
			} r0;
		struct _r1
			{
			int32_t		nInPulseA;
			int32_t		nInPulseB;
			} r1;
		struct _r3
			{
			uint32_t	nErrorBit;
			uint32_t	nWarnBit;
			} r28;
		struct _r4
			{
			uint32_t	nCtrlBit;
			uint32_t	nIOflag;
			} r29;
		struct _r5
			{
			int16_t		nOrderSpeedAY;
			int16_t		nOrderSpeedBX;
			int16_t		nBackSpeedAY;
			int16_t		nBackSpeedBX;
			} r2a;
		struct _r6
			{
			int16_t		nOrderCurrentAY;
			int16_t		nOrderCurrentBX;
			int16_t		nLoadA;
			int16_t		nLoadB;
			} r2b;
		struct _r7
			{
			int32_t		nInPulseA;
			int32_t		nInPulseB;
			} r2c;
		struct _r8
			{
			int16_t		nAnalog0;
			int16_t		nAnalog1;
			int16_t		nPowerMain;
			int16_t		nPowerCtrl;
			} r2d;
		struct _r9
			{
			float		nMonitor0;
			float		nMonitor1;
			} r2e;
		} u;
	} CANABH3_RESULT,*pCANABH3_RESULT;

#endif	//多重インクルード防止
