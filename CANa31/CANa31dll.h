#pragma once

//���̃t�@�C���́A���[�U�[���ł̂ݎg�p���܂�
//�iDLL�\�z���ɂ͕s�v�j

#include <Windows.h>
#include <stdint.h>
#include "typedef.h"

//============================================================
//�G�N�X�|�[�g����֐����ȒP�Ɉ����ׂ̗v�f
//============================================================

//�g�p�J�n���ɌĂяo���K�v���L��܂�
typedef void(*CA3_InitInstance)(int32_t);

//�g�p�I�����ɌĂяo���K�v���L��܂�
typedef void (*CA3_ExitInstance)(void);

//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
typedef int32_t(*CA3_GetInterfaceCount)(void);

//�w��C���^�[�t�F�[�X���J��
typedef int32_t(*CA3_OpenInterface)(int32_t);

//�J�����C���^�[�t�F�[�X�����
typedef void(*CA3_CloseInterface)(void);

//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
typedef int32_t(*CA3_GetCurrentInterface)(void);

//���݃C���^�[�t�F�[�X���J���Ă��邩�H
typedef int32_t(*CA3_IsOpenInterface)(void);

//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
typedef void(*CA3_SetOpenTimeout)(uint32_t);

//���M�^�C���A�E�g���Ԃ�ݒ�
typedef void(*CA3_SetSendTimeout)(uint32_t);

//��M�^�C���A�E�g���Ԃ�ݒ�
typedef void(*CA3_SetRecvTimeout)(uint32_t);

//�ʐM�Ώ�ABH3�̃A�h���X��ݒ�
typedef void(*CA3_SetTargetID)(uint8_t);

//�ʐM�Ώ�ABH3�̃A�h���X���擾
typedef uint8_t(*CA3_GetTargetID)(void);

//�ʐM�z�X�g�̃A�h���X��ݒ�
typedef void(*CA3_SetHostID)(uint8_t);

//�ʐM�z�X�g�̃A�h���X���擾
typedef uint8_t(*CA3_GetHostID)(void);

//�ʐM���x���w��
typedef void(*CA3_SetBaudrate)(uint32_t);

//�ݒ肵���ʐM���x���擾
typedef uint32_t(*CA3_GetBaudrate)(void);

//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
typedef uint32_t(*CA3_GetTm)(void);

//�w�߂̏�����
typedef int32_t(*CA3_abh3_can_init)(void);

//�w�߂̑��M�i���ʁj
typedef int32_t(*CA3_abh3_can_cmdAY)(int16_t,pCANABH3_RESULT);
typedef int32_t(*CA3_abh3_can_cmdBX)(int16_t,pCANABH3_RESULT);

//�w�߂̑��M�i�����j
typedef int32_t(*CA3_abh3_can_cmd)(int16_t,int16_t,pCANABH3_RESULT);

//���͂̑��M�i�ꊇ�j
typedef int32_t(*CA3_abh3_can_inSet)(int32_t,int32_t,pCANABH3_RESULT);

//���͂̑��M�i�r�b�g�j
typedef int32_t(*CA3_abh3_can_inBitSet)(int8_t,int8_t,pCANABH3_RESULT);

//�ώZ�l�̃��N�G�X�g
typedef int32_t(*CA3_abh3_can_reqPulse)(pCANABH3_RESULT);

//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
typedef int32_t(*CA3_abh3_can_reqBRD)(uint8_t,pCANABH3_RESULT);

//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
typedef int32_t(*CA3_abh3_can_trans)(char*,char*,uint32_t*);

//���x
typedef int16_t(*CA3_cnvVel2CAN)(float vel);
typedef float(*CA3_cnvCAN2Vel)(int16_t vel);

//�d��
typedef int16_t(*CA3_cnvCur2CAN)(float);
typedef float(*CA3_cnvCAN2Cur)(int16_t);

//���ח�
typedef float(*CA3_cnvCAN2Load)(int16_t);

//�A�i���O����
typedef float(*CA3_cnvCAN2Analog)(int16_t);

//�d���d��
typedef float(*CA3_cnvCAN2Volt)(int16_t);


//�f�o�b�O�@�\�̗L�������ݒ�
typedef int32_t(*CA3_SetDebugMode)(int32_t);

//�f�o�b�O���̃N���A
typedef int32_t(*CA3_ClearDebugInfo)(uint8_t);

//�f�o�b�O���̎擾
typedef int32_t(*CA3_GetDebugInfo)(int32_t,uint8_t*);



//============================================================
//DLL�̑S�֐��������ׂ̍\����
//============================================================

//DLL�t�@���N�V�����֐��A�h���X�Ǘ�
typedef struct _CA3_FUNCLIST
	{
	//�g�p�J�n���ɌĂяo���K�v���L��܂�
	CA3_InitInstance		InitInstance;
	//�g�p�I�����ɌĂяo���K�v���L��܂�
	CA3_ExitInstance		ExitInstance;
	//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
	CA3_GetInterfaceCount	GetInterfaceCount;
	//�w��C���^�[�t�F�[�X���J��
	CA3_OpenInterface		OpenInterface;
	//�J�����C���^�[�t�F�[�X�����
	CA3_CloseInterface		CloseInterface;
	//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
	CA3_GetCurrentInterface	GetCurrentInterface;
	//���݃C���^�[�t�F�[�X���J���Ă��邩�H
	CA3_IsOpenInterface		IsOpenInterface;
	//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
	CA3_SetOpenTimeout		SetOpenTimeout;
	//���M�^�C���A�E�g���Ԃ�ݒ�
	CA3_SetSendTimeout		SetSendTimeout;
	//��M�^�C���A�E�g���Ԃ�ݒ�
	CA3_SetRecvTimeout		SetRecvTimeout;
	//�ʐM�Ώ�ABH3�̃A�h���X��ݒ�
	CA3_SetTargetID			SetTargetID;
	//�ʐM�Ώ�ABH3�̃A�h���X���擾
	CA3_GetTargetID			GetTargetID;
	//�ʐM�z�X�g�̃A�h���X��ݒ�
	CA3_SetHostID			SetHostID;
	//�ʐM�z�X�g�̃A�h���X���擾
	CA3_GetHostID			GetHostID;
	//�ʐM���x���w��
	CA3_SetBaudrate			SetBaudrate;
	//�ݒ肵���ʐM���x���擾
	CA3_GetBaudrate			GetBaudrate;
	//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
	CA3_GetTm				GetTm;
	//�w�߂̏�����
	CA3_abh3_can_init		abh3_can_init;
	//�w�߂̑��M�i���ʁj
	CA3_abh3_can_cmdAY		abh3_can_cmdAY;
	CA3_abh3_can_cmdBX		abh3_can_cmdBX;
	//�w�߂̑��M�i�����j
	CA3_abh3_can_cmd		abh3_can_cmd;
	//���͂̑��M�i�ꊇ�j
	CA3_abh3_can_inSet		abh3_can_inSet;
	//���͂̑��M�i�r�b�g�j
	CA3_abh3_can_inBitSet	abh3_can_inBitSet;
	//�ώZ�l�̃��N�G�X�g
	CA3_abh3_can_reqPulse	abh3_can_reqPulse;
	//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
	CA3_abh3_can_reqBRD		abh3_can_reqBRD;
	//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
	CA3_abh3_can_trans		abh3_can_trans;
	//���x
	CA3_cnvVel2CAN			cnvVel2CAN;
	CA3_cnvCAN2Vel			cnvCAN2Vel;
	//�d��
	CA3_cnvCAN2Cur			cnvCAN2Cur;
	CA3_cnvCur2CAN			cnvCur2CAN;
	//���ח�
	CA3_cnvCAN2Load			cnvCAN2Load;
	//�A�i���O����
	CA3_cnvCAN2Analog		cnvCAN2Analog;
	//�d���d��
	CA3_cnvCAN2Volt			cnvCAN2Volt;

	//

	//�f�o�b�O�@�\�̗L�������ݒ�
	CA3_SetDebugMode		SetDebugMode;
	//�f�o�b�O���̃N���A
	CA3_ClearDebugInfo		ClearDebugInfo;
	//�f�o�b�O���̎擾
	CA3_GetDebugInfo		GetDebugInfo;

	} CA3_FUNCLIST,*pCA3_FUNCLIST;

//DLL�̊֐����ꊇ�ň����ׂ̊֐���`
int32_t GetFunctions(HMODULE hDLL,pCA3_FUNCLIST pFuncList);
