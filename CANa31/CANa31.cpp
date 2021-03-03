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

//�T�v
//	�{DLL�ŃG�N�X�|�[�g����֐����i�[���Ă���\�[�X�R�[�h

#include "stdafx.h"

//#define _NO_CANA31API_EXPORT	//��`�Ȃ��ŃG�N�X�|�[�g�A��`�L��ŃC���|�[�g
#include "CANa31.h"

//ABH3����N���X�ƃC���^�[�t�F�[�X�N���X�ւ̃|�C���^�i�O���[�o���j
static CAbh3* g_pAbh3 = NULL;
static CIxxatV2* g_pIF1 = NULL;
static CIxxatSimple* g_pIF2 = NULL;

//DLL�g�p�J�n���ɌĂяo���K�v���L��܂�
CANA31API void InitInstance(int32_t nIFnum)
	{
	//�T�v
	//	DLL�g�p�J�n���ɌĂяo���K�v���L��܂�
	//�p�����[�^
	//	nIFnum		�g�p�C���^�[�t�F�[�X�i0...IxxatV2  1...IxxatSimple�j
	//�߂�l
	//	����
	//���ӓ_
	//	�C���^�[�t�F�[�X��؂�ւ������ꍇ�́A�I�������̌�AExitInstance��
	//	�Ăяo���A���̌��InitInstance���ēx�g�p���ăC���^�[�t�F�[�X���w�肵�܂�
	//	�i�ڑ���Ԃł̐؂�ւ��s�j

	//
	ExitInstance();
	//
	if(nIFnum == 0)
		{
		g_pIF1 = new CIxxatV2();
		g_pAbh3 = new CAbh3(g_pIF1);
		}
	else if(nIFnum == 1)
		{
		g_pIF2 = new CIxxatSimple();
		g_pAbh3 = new CAbh3(g_pIF2);
		}
	}

//DLL�g�p�I�����ɌĂяo���K�v���L��܂�
CANA31API void ExitInstance()
	{
	//�T�v
	//	DLL�g�p�I�����ɌĂяo���K�v���L��܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	����
	//���ӓ_
	//	DLL�J�����O�ŌĂяo���ĉ�����

	if(g_pAbh3)
		{
		delete g_pAbh3;
		g_pAbh3 = NULL;
		}
	if(g_pIF1)
		{
		delete g_pIF1;
		g_pIF1 = NULL;
		}
	if(g_pIF2)
		{
		delete g_pIF2;
		g_pIF2 = NULL;
		}
	}

//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
CANA31API int32_t GetInterfaceCount()
	{
	return(g_pAbh3->GetInterfaceCount());
	}

//�w��C���^�[�t�F�[�X���J��
CANA31API int32_t OpenInterface(int32_t nDeviceNum)
	{
	return(g_pAbh3->OpenInterface(nDeviceNum));
	}

//�J�����C���^�[�t�F�[�X�����
CANA31API void CloseInterface()
	{
	g_pAbh3->CloseInterface();
	}

//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
CANA31API int32_t GetCurrentInterface()
	{
	return(g_pAbh3->GetCurrentInterface());
	}

//���݃C���^�[�t�F�[�X���J���Ă��邩�H
CANA31API int32_t IsOpenInterface()
	{
	return(g_pAbh3->IsOpenInterface());
	}

//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
CANA31API void SetOpenTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetOpenTimeout(nTimeoutMS);
	}

//���M�^�C���A�E�g���Ԃ�ݒ�
CANA31API void SetSendTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetSendTimeout(nTimeoutMS);
	}

//��M�^�C���A�E�g���Ԃ�ݒ�
CANA31API void SetRecvTimeout(uint32_t nTimeoutMS)
	{
	g_pAbh3->SetRecvTimeout(nTimeoutMS);
	}

//�ʐM�Ώ�ABH3�̃A�h���X��ݒ�
CANA31API void SetTargetID(uint8_t nAdrs)
	{
	g_pAbh3->SetTargetID(nAdrs);
	}

//�ʐM�Ώ�ABH3�̃A�h���X���擾
CANA31API uint8_t GetTargetID()
	{
	return(g_pAbh3->GetTargetID());
	}

//�ʐM�z�X�g�̃A�h���X��ݒ�
CANA31API void SetHostID(uint8_t nAdrs)
	{
	g_pAbh3->SetHostID(nAdrs);
	}

//�ʐM�z�X�g�̃A�h���X���擾
CANA31API uint8_t GetHostID(void)
	{
	return(g_pAbh3->GetHostID());
	}

//�ʐM���x���w��
CANA31API void SetBaudrate(uint32_t nBaudrateKbps)
	{
	g_pAbh3->SetBaudrate(nBaudrateKbps);
	}

//�ݒ肵���ʐM���x���擾
CANA31API uint32_t GetBaudrate()
	{
	return(g_pAbh3->GetBaudrate());
	}

//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
CANA31API uint32_t GetTm()
	{
	return(g_pAbh3->GetTm());
	}

//�w�߂̏�����
CANA31API int32_t abh3_can_init()
	{
	return(g_pAbh3->abh3_can_init());
	}

//�w�߂̑��M�i���ʁj
CANA31API int32_t abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_cmdAY(cmd,pPtr));
	}
CANA31API int32_t abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_cmdBX(cmd,pPtr));
	}

//�w�߂̑��M�i�����j
CANA31API int32_t abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_cmd(cmdAY,cmdBX,pPtr));
	}

//���͂̑��M�i�ꊇ�j
CANA31API int32_t abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_inSet(data,mask,pPtr));
	}

//���͂̑��M�i�r�b�g�j
CANA31API int32_t abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_inBitSet(num,data,pPtr));
	}

//�ώZ�l�̃��N�G�X�g
CANA31API int32_t abh3_can_reqPulse(pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_reqPulse(pPtr));
	}

//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
CANA31API int32_t abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr)
	{
	return(g_pAbh3->abh3_can_reqBRD(num,pPtr));
	}

//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
CANA31API int32_t abh3_can_trans(char* sbuf,char* rbuf,uint32_t* rbuflen)
	{
	//���ӓ_
	//	�A�v����DLL��HEAP������ɂȂ�Ȃ��悤�Ȏg�����i��FMFC�A�v���Ŏg�p�j��
	//	�����ꍇ�ADLL���Ŋm�ۂ����o�b�t�@���A�v�����ŊJ���o���Ȃ�(ASSERT�����)�ׁA
	//	�����ŃA�v�����̃o�b�t�@��DLL���̃o�b�t�@���e���R�s�[����
	//
	//�p�����[�^
	//	sbuf		���M�f�[�^(ANSI)
	//	rbuf		��M�f�[�^�i�[�̈�i�A�v�����Ŋm�ۂ���j
	//	rbuflen		��M�f�[�^�i�[�̈�̃T�C�Y�i�A�v�����Őݒ肵�A�{�֐��Ŋi�[�T�C�Y������j
	//�߂�l
	//	0			����
	//	-1			�ُ�
	//	-2			�o�b�t�@�T�C�Y������Ȃ�
	
	//
	char* pTmp = NULL;
	size_t nTmpLen = 0;
	int32_t nResult = g_pAbh3->abh3_can_trans(sbuf,pTmp,nTmpLen);
	//����H
	if(nResult == 0)
		{
		//�Œ���K�v�ȃT�C�Y��ݒ�
		*rbuflen = (uint32_t)nTmpLen;

		//�T�C�Y�������H
		if(*rbuflen >= nTmpLen)
			{
			::CopyMemory(rbuf,pTmp,nTmpLen);
			}
		else
			{
			//����Ȃ�
			nResult = -2;		//�̈�T�C�Y������Ȃ�����
			}
		//���p���Ă���ׁA�J�����K�v
		delete pTmp;
		}
	//
	return(nResult);
	}

//���x
CANA31API int16_t	cnvVel2CAN(float vel)
	{
	return(g_pAbh3->cnvVel2CAN(vel));
	}
CANA31API float	cnvCAN2Vel(int16_t vel)
	{
	return(g_pAbh3->cnvCAN2Vel(vel));
	}

//�d���l
CANA31API int16_t cnvCur2CAN(float cur)
	{
	return(g_pAbh3->cnvCur2CAN(cur));
	}
CANA31API float	cnvCAN2Cur(int16_t cur)
	{
	return(g_pAbh3->cnvCAN2Cur(cur));
	}

//���ח�
CANA31API float	cnvCAN2Load(int16_t load)
	{
	return(g_pAbh3->cnvCAN2Load(load));
	}

//�A�i���O����
CANA31API float	cnvCAN2Analog(int16_t analog)
	{
	return(g_pAbh3->cnvCAN2Analog(analog));
	}

//�d���d��
CANA31API float	cnvCAN2Volt(int16_t volt)
	{
	return(g_pAbh3->cnvCAN2Volt(volt));
	}





//�f�o�b�O�@�\�̗L�������ݒ�
CANA31API int32_t SetDebugMode(int32_t nEnable)
	{
	g_pAbh3->SetDebugMode((bool)(nEnable != 0));
	return(0);
	}

//�f�o�b�O���̃N���A
CANA31API int32_t ClearDebugInfo(uint8_t nDummy)
	{
	g_pAbh3->ClearDebugInfo();
	return(0);
	}

//�f�o�b�O���̎擾
CANA31API int32_t GetDebugInfo(int32_t nPt,uint8_t* pMemory9)
	{
	uint8_t* pInfo = g_pAbh3->GetDebugInfo(nPt);
	if(pInfo == NULL)
		return(1);
	//
	::memcpy(pMemory9,pInfo,9);
	return(0);
	}
