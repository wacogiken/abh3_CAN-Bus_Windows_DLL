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
//�G�N�X�|�[�g�H
//============================================================

 #ifndef _NO_CANA31API_EXPORT
  #define CANA31API __declspec(dllexport)
#else
  #define CANA31API __declspec(dllimport)
#endif

//============================================================
//�ȉ������P�[�W��C����`���Ƃ���
//============================================================

#ifdef __cplusplus
 extern "C" {
#endif //__cplusplus

//============================================================
//���[�U�[�p�֐�
//============================================================

//�g�p�J�n���ɌĂяo���K�v���L��܂�
CANA31API void InitInstance(int32_t nIFnum);

//�g�p�I�����ɌĂяo���K�v���L��܂�
CANA31API void ExitInstance(void);

//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
CANA31API int32_t GetInterfaceCount(void);

//�w��C���^�[�t�F�[�X���J��
CANA31API int32_t OpenInterface(int32_t nDeviceNum);

//�J�����C���^�[�t�F�[�X�����
CANA31API void CloseInterface(void);

//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
CANA31API int32_t GetCurrentInterface(void);

//���݃C���^�[�t�F�[�X���J���Ă��邩�H
CANA31API int32_t IsOpenInterface(void);

//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
CANA31API void SetOpenTimeout(uint32_t nTimeoutMS);

//���M�^�C���A�E�g���Ԃ�ݒ�
CANA31API void SetSendTimeout(uint32_t nTimeoutMS);

//��M�^�C���A�E�g���Ԃ�ݒ�
CANA31API void SetRecvTimeout(uint32_t nTimeoutMS);

//�ʐM�Ώ�ABH3�̃A�h���X��ݒ�
CANA31API void SetTargetID(uint8_t nAdrs);

//�ʐM�Ώ�ABH3�̃A�h���X���擾
CANA31API uint8_t GetTargetID(void);

//�ʐM�z�X�g�̃A�h���X��ݒ�
CANA31API void SetHostID(uint8_t nAdrs);

//�ʐM�z�X�g�̃A�h���X���擾
CANA31API uint8_t GetHostID(void);

//�ʐM���x���w��
CANA31API void SetBaudrate(uint32_t nBaudrateKbps);

//�ݒ肵���ʐM���x���擾
CANA31API uint32_t GetBaudrate(void);

//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
CANA31API uint32_t GetTm(void);

//�w�߂̏�����
CANA31API int32_t abh3_can_init(void);

//�w�߂̑��M�i���ʁj
CANA31API int32_t abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr);
CANA31API int32_t abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr);

//�w�߂̑��M�i�����j
CANA31API int32_t abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr);

//���͂̑��M�i�ꊇ�j
CANA31API int32_t abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr);

//���͂̑��M�i�r�b�g�j
CANA31API int32_t abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr);

//�ώZ�l�̃��N�G�X�g
//2021.05.14 �@�\�������ׁ̈A�폜
//CANA31API int32_t abh3_can_reqPulse(pCANABH3_RESULT pPtr);

//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
CANA31API int32_t abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr);

//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
CANA31API int32_t abh3_can_trans(char* sbuf,char* rbuf,uint32_t* rbuflen);

//���x��������l�֕ϊ�
CANA31API int16_t cnvVel2CAN(float vel);

//�����l���瑬�x�֕ϊ�
CANA31API float	cnvCAN2Vel(int16_t vel);

//�d���l��������l�ɕϊ�
CANA31API int16_t cnvCur2CAN(float cur);

//�����l����d���l�ɕϊ�
CANA31API float	cnvCAN2Cur(int16_t cur);

//�����l���畉�ח��ɕϊ�
CANA31API float	cnvCAN2Load(int16_t load);

//�����l����A�i���O���͂ɕϊ�
CANA31API float	cnvCAN2Analog(int16_t analog);

//�����l����d���d���ɕϊ�
CANA31API float	cnvCAN2Volt(int16_t volt);



//============================================================
//�f�o�b�O�p
//============================================================

//�f�o�b�O�@�\�̗L�������ݒ�
CANA31API int32_t SetDebugMode(int32_t nEnable);

//�f�o�b�O���̃N���A
CANA31API int32_t ClearDebugInfo(uint8_t nDummy);

//�f�o�b�O���̎擾
CANA31API int32_t GetDebugInfo(int32_t nPt,uint8_t* pMemory9);


//============================================================
//�����P�[�W����
//============================================================
#ifdef __cplusplus
 }
#endif //__cplusplus
