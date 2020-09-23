#pragma once

//���̃t�@�C���́A���[�U�[���ł̂ݎg�p���܂�
//�iDLL�\�z���ɂ͕s�v�j

#include "stdafx.h"
#include "CANa31dll.h"

//DLL����֐��̃G���g�����擾
static FARPROC GetDllFunc(HMODULE hModule,char* pFuncName);

//
int32_t GetFunctions(HMODULE hDLL,pCA3_FUNCLIST pFuncList)
	{
	if (hDLL && pFuncList)
		{
		//�g�p�J�n���ɌĂяo���K�v���L��܂�
		pFuncList->InitInstance			= (CA3_InitInstance)GetDllFunc(hDLL,"InitInstance");
		//�g�p�I�����ɌĂяo���K�v���L��܂�
		pFuncList->ExitInstance			= (CA3_ExitInstance)GetDllFunc(hDLL,"ExitInstance");
		//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
		pFuncList->GetInterfaceCount	= (CA3_GetInterfaceCount)GetDllFunc(hDLL,"GetInterfaceCount");
		//�w��C���^�[�t�F�[�X���J��
		pFuncList->OpenInterface		= (CA3_OpenInterface)GetDllFunc(hDLL,"OpenInterface");
		//�J�����C���^�[�t�F�[�X�����
		pFuncList->CloseInterface		= (CA3_CloseInterface)GetDllFunc(hDLL,"CloseInterface");
		//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
		pFuncList->GetCurrentInterface	= (CA3_GetCurrentInterface)GetDllFunc(hDLL,"GetCurrentInterface");
		//���݃C���^�[�t�F�[�X���J���Ă��邩�H
		pFuncList->IsOpenInterface		= (CA3_IsOpenInterface)GetDllFunc(hDLL,"IsOpenInterface");
		//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
		pFuncList->SetOpenTimeout		= (CA3_SetOpenTimeout)GetDllFunc(hDLL,"SetOpenTimeout");
		//���M�^�C���A�E�g���Ԃ�ݒ�
		pFuncList->SetSendTimeout		= (CA3_SetSendTimeout)GetDllFunc(hDLL,"SetSendTimeout");
		//��M�^�C���A�E�g���Ԃ�ݒ�
		pFuncList->SetRecvTimeout		= (CA3_SetRecvTimeout)GetDllFunc(hDLL,"SetRecvTimeout");
		//�ʐM�Ώ�ABH3�̃A�h���X��ݒ�
		pFuncList->SetTargetID			= (CA3_SetTargetID)GetDllFunc(hDLL,"SetTargetID");
		//�ʐM�Ώ�ABH3�̃A�h���X���擾
		pFuncList->GetTargetID			= (CA3_GetTargetID)GetDllFunc(hDLL,"GetTargetID");
		//�ʐM�z�X�g�̃A�h���X��ݒ�
		pFuncList->SetHostID			= (CA3_SetHostID)GetDllFunc(hDLL,"SetHostID");
		//�ʐM�z�X�g�̃A�h���X���擾
		pFuncList->GetHostID			= (CA3_GetHostID)GetDllFunc(hDLL,"GetHostID");
		//�ʐM���x���w��
		pFuncList->SetBaudrate			= (CA3_SetBaudrate)GetDllFunc(hDLL,"SetBaudrate");
		//�ݒ肵���ʐM���x���擾
		pFuncList->GetBaudrate			= (CA3_GetBaudrate)GetDllFunc(hDLL,"GetBaudrate");
		//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
		pFuncList->GetTm				= (CA3_GetTm)GetDllFunc(hDLL,"GetTm");
		//�w�߂̏�����
		pFuncList->abh3_can_init		= (CA3_abh3_can_init)GetDllFunc(hDLL,"abh3_can_init");
		//�w�߂̑��M�i���ʁj
		pFuncList->abh3_can_cmdAY		= (CA3_abh3_can_cmdAY)GetDllFunc(hDLL,"abh3_can_cmdAY");
		pFuncList->abh3_can_cmdBX		= (CA3_abh3_can_cmdBX)GetDllFunc(hDLL,"abh3_can_cmdBX");
		//�w�߂̑��M�i�����j
		pFuncList->abh3_can_cmd			= (CA3_abh3_can_cmd)GetDllFunc(hDLL,"abh3_can_cmd");
		//���͂̑��M�i�ꊇ�j
		pFuncList->abh3_can_inSet		= (CA3_abh3_can_inSet)GetDllFunc(hDLL,"abh3_can_inSet");
		//���͂̑��M�i�r�b�g�j
		pFuncList->abh3_can_inBitSet	= (CA3_abh3_can_inBitSet)GetDllFunc(hDLL,"abh3_can_inBitSet");
		//�ώZ�l�̃��N�G�X�g
		pFuncList->abh3_can_reqPulse	= (CA3_abh3_can_reqPulse)GetDllFunc(hDLL,"abh3_can_reqPulse");
		//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
		pFuncList->abh3_can_reqBRD		= (CA3_abh3_can_reqBRD)GetDllFunc(hDLL,"abh3_can_reqBRD");
		//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
		pFuncList->abh3_can_trans		= (CA3_abh3_can_trans)GetDllFunc(hDLL,"abh3_can_trans");
		//���x
		pFuncList->cnvVel2CAN			= (CA3_cnvVel2CAN)GetDllFunc(hDLL,"cnvVel2CAN");
		pFuncList->cnvCAN2Vel			= (CA3_cnvCAN2Vel)GetDllFunc(hDLL,"cnvCAN2Vel");
		//�g���N
		pFuncList->cnvCAN2Trq			= (CA3_cnvCAN2Trq)GetDllFunc(hDLL,"cnvCAN2Trq");
		//���ח�
		pFuncList->cnvCAN2Load			= (CA3_cnvCAN2Load)GetDllFunc(hDLL,"cnvCAN2Load");
		//�A�i���O����
		pFuncList->cnvCAN2Analog		= (CA3_cnvCAN2Analog)GetDllFunc(hDLL,"cnvCAN2Analog");
		//�d���d��
		pFuncList->cnvCAN2Volt			= (CA3_cnvCAN2Volt)GetDllFunc(hDLL,"cnvCAN2Volt");
		}
	return(0);
	}

//DLL����֐��̃G���g�����擾
FARPROC GetDllFunc(HMODULE hModule,char* pFuncName)
	{
	FARPROC nResult = GetProcAddress(hModule,pFuncName);
	return(nResult);
	}

