//���d�C���N���[�h�h�~
#ifndef _TYPEDEF_H_INCLUDED_
#define _TYPEDEF_H_INCLUDED_

//�v���b�g�t�H�[���ˑ�
#include <Windows.h>
#include <stdint.h>


//================================================================================
//���[�U�[�����p�\�Ȍ^
//================================================================================

//��M�Ɏg�p����\���́i��M���{��M�f�[�^8�o�C�g�j
typedef struct _CANABH3_RESULT
	{
	//��M�f�[�^��CAN-ID
	uint32_t	nID;

	//���J�p�i���M�v�f�j
	struct _DP0S
		{
		int16_t		nOrderAY;
		int16_t		nOrderBX;
		int32_t		nInputBit;
		} DP0S;

	union
		{
		//�����p
		uint8_t		raw[8];

		//���J�p�i��M�v�f�j
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

#endif	//���d�C���N���[�h�h�~
