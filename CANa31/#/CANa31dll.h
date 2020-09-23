#pragma once

#include <Windows.h>
#include <stdint.h>

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
typedef int32_t(*CA3_abh3_can_trans)(char*,char*&,size_t&);

//���x
typedef int16_t(*CA3_cnvVel2CAN)(float vel);
typedef float(*CA3_cnvCAN2Vel)(int16_t vel);

//�g���N
typedef float(*CA3_cnvCAN2Trq)(int16_t);

//���ח�
typedef float(*CA3_cnvCAN2Load)(int16_t);

//�A�i���O����
typedef float(*CA3_cnvCAN2Analog)(int16_t);

//�d���d��
typedef float(*CA3_cnvCAN2Volt)(int16_t);

//============================================================
//DLL�̑S�֐��������ׂ̍\����
//============================================================

//DLL�t�@���N�V�����֐��A�h���X�Ǘ�
typedef struct _CA3_FUNCLIST
	{
	//�g�p�J�n���ɌĂяo���K�v���L��܂�
	CA3_InitInstance		hCA3_InitInstance;
	//�g�p�I�����ɌĂяo���K�v���L��܂�
	CA3_ExitInstance		hCA3_ExitInstance;
	//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
	CA3_GetInterfaceCount	hCA3_GetInterfaceCount;
	//�w��C���^�[�t�F�[�X���J��
	CA3_OpenInterface		hCA3_OpenInterface;
	//�J�����C���^�[�t�F�[�X�����
	CA3_CloseInterface		hCA3_CloseInterface;
	//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
	CA3_GetCurrentInterface	hCA3_GetCurrentInterface;
	//���݃C���^�[�t�F�[�X���J���Ă��邩�H
	CA3_IsOpenInterface		hCA3_IsOpenInterface;
	//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
	CA3_SetOpenTimeout		hCA3_SetOpenTimeout;
	//���M�^�C���A�E�g���Ԃ�ݒ�
	CA3_SetSendTimeout		hCA3_SetSendTimeout;
	//��M�^�C���A�E�g���Ԃ�ݒ�
	CA3_SetRecvTimeout		hCA3_SetRecvTimeout;
	//�ʐM�Ώ�ABH3�̃A�h���X��ݒ�
	CA3_SetTargetID			hCA3_SetTargetID;
	//�ʐM�Ώ�ABH3�̃A�h���X���擾
	CA3_GetTargetID			hCA3_GetTargetID;
	//�ʐM�z�X�g�̃A�h���X��ݒ�
	CA3_SetHostID			hCA3_SetHostID;
	//�ʐM�z�X�g�̃A�h���X���擾
	CA3_GetHostID			hCA3_GetHostID;
	//�ʐM���x���w��
	CA3_SetBaudrate			hCA3_SetBaudrate;
	//�ݒ肵���ʐM���x���擾
	CA3_GetBaudrate			hCA3_GetBaudrate;
	//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
	CA3_GetTm				hCA3_GetTm;
	//�w�߂̏�����
	CA3_abh3_can_init		hCA3_abh3_can_init;
	//�w�߂̑��M�i���ʁj
	CA3_abh3_can_cmdAY		hCA3_abh3_can_cmdAY;
	CA3_abh3_can_cmdBX		hCA3_abh3_can_cmdBX;
	//�w�߂̑��M�i�����j
	CA3_abh3_can_cmd		hCA3_abh3_can_cmd;
	//���͂̑��M�i�ꊇ�j
	CA3_abh3_can_inSet		hCA3_abh3_can_inSet;
	//���͂̑��M�i�r�b�g�j
	CA3_abh3_can_inBitSet	hCA3_abh3_can_inBitSet;
	//�ώZ�l�̃��N�G�X�g
	CA3_abh3_can_reqPulse	hCA3_abh3_can_reqPulse;
	//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
	CA3_abh3_can_reqBRD		hCA3_abh3_can_reqBRD;
	//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
	CA3_abh3_can_trans		hCA3_abh3_can_trans;
	//���x
	CA3_cnvVel2CAN			hCA3_cnvVel2CAN;
	CA3_cnvCAN2Vel			hCA3_cnvCAN2Vel;
	//�g���N
	CA3_cnvCAN2Trq			hCA3_cnvCAN2Trq;
	//���ח�
	CA3_cnvCAN2Load			hCA3_cnvCAN2Load;
	//�A�i���O����
	CA3_cnvCAN2Analog		hCA3_cnvCAN2Analog;
	//�d���d��
	CA3_cnvCAN2Volt			hCA3_cnvCAN2Volt;
	} CA3_FUNCLIST,*pCA3_FUNCLIST;

//DLL�̊֐����ꊇ�ň����ׂ̊֐���`
typedef int32_t(*CA3_GetFunctions)(HMODULE,pCA3_FUNCLIST);
