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
		//==========
		//���[�U�[�p
		//==========

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
		//�d��
		pFuncList->cnvCur2CAN			= (CA3_cnvCur2CAN)GetDllFunc(hDLL,"cnvCur2CAN");
		pFuncList->cnvCAN2Cur			= (CA3_cnvCAN2Cur)GetDllFunc(hDLL,"cnvCAN2Cur");
		//���ח�
		pFuncList->cnvCAN2Load			= (CA3_cnvCAN2Load)GetDllFunc(hDLL,"cnvCAN2Load");
		//�A�i���O����
		pFuncList->cnvCAN2Analog		= (CA3_cnvCAN2Analog)GetDllFunc(hDLL,"cnvCAN2Analog");
		//�d���d��
		pFuncList->cnvCAN2Volt			= (CA3_cnvCAN2Volt)GetDllFunc(hDLL,"cnvCAN2Volt");


		//==========
		//�f�o�b�O�p
		//==========

		//�f�o�b�O�@�\�̗L������
		pFuncList->SetDebugMode			= (CA3_SetDebugMode)GetDllFunc(hDLL,"SetDebugMode");
		//�f�o�b�O���̃N���A
		pFuncList->ClearDebugInfo		= (CA3_ClearDebugInfo)GetDllFunc(hDLL,"ClearDebugInfo");
		//�f�o�b�O���̎擾
		pFuncList->GetDebugInfo			= (CA3_GetDebugInfo)GetDllFunc(hDLL,"GetDebugInfo");

		}
	return(0);
	}

//DLL����֐��̃G���g�����擾
FARPROC GetDllFunc(HMODULE hModule,char* pFuncName)
	{
	FARPROC nResult = GetProcAddress(hModule,pFuncName);
	return(nResult);
	}

