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
//========================================
//�T�v
//========================================
//	Ixxat Simple�̃f�o�C�X���g���ׂ̃N���X
//
//========================================
//����
//========================================
//	2020.09.xx	yo0043	1st release
//	2020.10.01	yo0043	2nd release
//						simply_open���A����COM�|�[�g(not CAN)���J�����ꍇ��
//						��xsimply_close���Ăяo���Ȃ��ƊJ���Ȃ��Ȃ���ɑΏ�
//
//========================================
//�����ɕK�v�ȑO����
//========================================
//	IxxatSimple�p�̃h���C�o���_�E�����[�h���ēW�J�iSDK�������Ă���j
//	�v���W�F�N�g�ݒ�ŁA�ǉ��C���N���[�h�t�H���_�Ɉȉ���ǉ�
//		"<�W�J�����t�H���_>\inc"
//	�v���W�F�N�g�ݒ�ŁA�ǉ����C�u�����t�H���_�Ɉȉ���ǉ�
//		"<�W�J�����t�H���_>\lib\dll"
//	�v���W�F�N�g�̎��s�t�@�C�����i�[�����ꏊ�ɁA��L���C�u�����t�H���_����DLL���R�s�[
//
//========================================
//�g�p���@
//========================================
//	��{�I�Ȏg�����́A�T���v���\�[�X�����ĉ�����
//
//========================================
//�f�o�C�X�ŗL
//========================================
//�G���[������
//	��{�I�Ɋ֐��ŃG���[������������A�����ؒf���A�ڑ����鏊����
//	��蒼���𐄏����܂��B�i���Z�b�g�őS�v�f��������̂ŁA���ǌq�����������K�{�ƂȂ�ׁj
//
//�{�[���[�g�ɂ͈ȉ��̒l�݂̂��ݒ�\�ł�
//		�l		�ݒ�			�l		�ݒ�	
//	---------------------------------------------			
//		10	->	10Kbps			20	->	20Kbps
//		50	->	50Kbps			100	->	100Kbps
//		125	->	125Kbps			250	->	250Kbps
//		500	->	500Kbps			800	->	800Kbps
//		1000->	1000Kbps
//	---------------------------------------------			
//
//OnGetInterfaceCount�̖߂�l
//	���1���߂�܂�
//
//OnOpenInterface�ɓn���f�o�C�X�̔ԍ�
//	COM�|�[�g�ԍ�(1�J�n)��n���ĉ�����
//
//SetTimeout_Open,SetTimeout�֐��Ŏw�肷�������J�����̃^�C���A�E�g�l
//	�ݒ�͖�������܂�
//
//SetTimeout_Send,SetTimeout�֐��Ŏw�肷�鑗�M���̃^�C���A�E�g�l
//	�ݒ�͖�������܂�
//
//OnCanGetError�Ŗ߂�G���[�X�e�[�^�X�́ACAN��M���s�������ɃG���[��
//�Ȃ�Ȃ�����A�l���X�V����܂���i�񓯊��ōX�V����Ȃ��j
//
//========================================

//�R���\�[���ւ̃f�o�b�O�o�́i���M�y�ю�M�j0�ȊO�ŗL��
#define	DEBUGPRINT	(0)

//
#include "stdafx.h"
#include "IxxatSimple.h"
#include <process.h>
#include <stdio.h>

//�{�N���X�ň����f�o�C�X�ɕK�{�̃��C�u����
#pragma comment(lib,"simplyCAN.lib")

//���S�ȉ��
template <class T> void SafeRelease(T **ppT)
	{
	if (*ppT)
		{
		(*ppT)->Release();
		*ppT = NULL;
		}
	}

//���S�ȍ폜
template <class T> void SafeDelete(T **ppT)
	{
	if (*ppT)
		{
		delete *ppT;
		*ppT = NULL;
		}
	}

//�R���X�g���N�^
CIxxatSimple::CIxxatSimple()
	{
	//�X�e�[�^�X�ۑ��p�̈��������
	memset(&m_status,0,sizeof(IXXATSMPL_STATUS));
	//���̑�������
	m_hReadThread = NULL;
	m_bQuitThread = false;
	m_nCanReadPt = 0;
	m_nCanWritePt = 0;
	m_pCanMsg = NULL;

	//�r������p�Z�}�t�H
	m_hSema = ::CreateSemaphore(NULL,1,1,NULL);
	m_hSema2 = ::CreateSemaphore(NULL,1,1,NULL);
	}

//�f�X�g���N�^
CIxxatSimple::~CIxxatSimple()
	{
	//������J���Ă���ꍇ���L��̂ŕ��鏈�����s��
	OnCloseInterface();

	//�r������p�Z�}�t�H�J��
	if(m_hSema)
		{
		::CloseHandle(m_hSema);
		m_hSema = NULL;
		}
	if(m_hSema2)
		{
		::CloseHandle(m_hSema2);
		m_hSema2 = NULL;
		}

	}

//================================================================================
//�g�pCAN�C���^�[�t�F�[�X���Ɏ������K�{�̉��z�֐�
//================================================================================

//���p�\��CAN�C���^�[�t�F�[�X�����擾����ꍇ�ɌĂяo����܂�
int32_t CIxxatSimple::OnGetInterfaceCount()
	{
	//�T�v
	//	�w��CAN�C���^�[�t�F�[�X�̗��p�\�����擾����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0			���p�\�C���^�[�t�F�[�X������܂���
	//	-1			�������ɃG���[���������܂���
	//	��L�ȊO	���p�\��CAN�C���^�[�t�F�[�X��
	//

	//todo:CAN�C���^�[�t�F�[�X�̐���߂��������������ĉ�����

	//����
	return(1);
	}

//�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
int32_t CIxxatSimple::OnOpenInterface(int32_t nDeviceNum)
	{
	//�T�v
	//	�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	nDeviceNum		1�J�n��COM�|�[�g�ԍ�
	//�߂�l
	//	0				����I��
	//	0�ȊO			�ُ�I��(simply_get_last_error�̖߂�l�Q��)

	//todo:CAN�C���^�[�t�F�[�X���J���������������ĉ�����

	int nResult = -1;

	//�J���Ă���ꍇ���L��̂ŕ���
	OnCloseInterface();

	//�r������J�n
	if(!LockCanMsg())
		return(-1);	//������L���s

	//
	do
		{
		//�f�o�C�X�����\�z���ĉ�����J��
		char* pDeviceName = new char[16]();
		sprintf_s(pDeviceName,16,"COM%d",nDeviceNum);
		bool bResult = simply_open(pDeviceName);
		delete pDeviceName;
		if(!bResult)
			{
			//���s
			simply_close();		//����COM�|�[�g���Ԉ���ĊJ�����ꍇ�A�����I�ɊJ�����܂܂ɂȂ�͗l��
								//�����I�ɕ��Ȃ��Ǝ����simply_open�Ő�L����Ă��鈵���ƂȂ�
			//�����G���[�R�[�h��߂�
			nResult = (int32_t)simply_get_last_error();
			break;
			}

		//����̏�����
		if(!simply_initialize_can(uint16_t(GetBaudrate())))
			{
			//���s
			simply_close();
			//�����G���[�R�[�h��߂�
			nResult = (int32_t)simply_get_last_error();
			break;
			}

		//CAN�̊J�n
		if(!simply_start_can())
			{
			//���s
			simply_reset_can();
			simply_close();
			//�����G���[�R�[�h��߂�
			nResult = (int32_t)simply_get_last_error();
			break;
			}

		//�X�e�[�^�X�̏�����
		m_status.err.nRaw = 0;

		//��M�f�[�^�i�[�p�̈�̏�����
		m_pCanMsg = new can_msg_t[256]();
		m_nCanReadPt = 0;
		m_nCanWritePt = 0;

		//��M�X���b�h�J�n
		m_bQuitThread = false;
		::SetThreadPriority(m_hReadThread = (HANDLE)_beginthreadex(NULL,0,&CIxxatSimple::ReceiveThread,this,NULL,&m_nThreadID),THREAD_PRIORITY_NORMAL);

		//�ڑ��ς݃t���O�ݒ�
		m_status.bOpen = true;

		//
		nResult = 0;
		break;
		} while(1);

	//�r������J��
	UnlockCanMsg();

	//��������
	return(nResult);
	}

//�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
void CIxxatSimple::OnCloseInterface()
	{
	//�T�v
	//	�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	����

	//todo:CAN�C���^�[�t�F�[�X���J���Ă���ꍇ�ɕ��鏈�����������ĉ�����

	//�r������
	if(!LockCanMsg())
		return;	//������L���s

	//��M�X���b�h���쒆�H
	if (m_hReadThread)
		{
		//��M�X���b�h��~
		m_bQuitThread = true;
		::WaitForSingleObject(m_hReadThread,INFINITE);
		::CloseHandle(m_hReadThread);
		m_hReadThread = NULL;
		}

	//������J���Ă���H
	if(m_status.bOpen)
		{
		simply_stop_can();
		simply_reset_can();
		simply_close();
		m_status.bOpen = false;
		}

	//��M�o�b�t�@�J��
	SafeDelete(&m_pCanMsg);

	//�r������J��
	UnlockCanMsg();
	}

//CAN�C���^�[�t�F�[�X�����M����ꍇ�ɌĂяo����܂�
int32_t CIxxatSimple::OnCanRecv(uint32_t* pCanID,uint8_t* pData8)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X����1���̃p�P�b�g����M���܂�
	//�p�����[�^
	//	pCanID			��M�p�P�b�g��ID���i�[����̈�ւ̃|�C���^
	//	pData8			��M�p�P�b�g���i�[����8[bytes]�̈�ւ̃|�C���^
	//	nTimeoutMS		��M���e����[ms]
	//�߂�l
	//	0				����I��
	//	0�ȊO			�^�C���A�E�g

	//todo:CAN�C���^�[�t�F�[�X����1�񕪂̎�M���s���������������ĉ�����

	//���ӓ_
	//	GetCanMsg�֐����o�b�t�@�̐�L���s���Ă���ׁA�{�֐�����
	//	�o�b�t�@�̐�L���s���Ă͂Ȃ�Ȃ�

	//�߂�l
	int32_t nResult = -1;

	//��M�^�C���A�E�g��Ύ��Ԃ��Z�o
	uint32_t nLimitTime = GetTm() + GetTimeout_Recv();

	//�������ԓ��̂ݏ�������i�A���Œ�1��͏�������j
	do
		{
		can_msg_t msg;
		if (GetCanMsg(&msg) == 0)
			{
			//����擾
			*pCanID = uint32_t(msg.ident);
			::memcpy(pData8,msg.payload,sizeof(char) * 8);
			nResult = 0;
			break;
			}
		else
			{
			Sleep(1);
			}
		} while(GetTm() < nLimitTime);

	//
	return(nResult);
	}

//CAN�C���^�[�t�F�[�X�ɑ��M����ꍇ�ɌĂяo����܂�
int32_t CIxxatSimple::OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X��1���̃p�P�b�g�𑗐M���܂�
	//�p�����[�^
	//	nCanID			���M�p�P�b�g��ID
	//	pData8			���M�p�P�b�g���i�[����Ă���8[bytes]�̈�ւ̃|�C���^
	//	nTimeoutMS		���M���e����[ms]
	//�߂�l
	//	0				����I��
	//	0�ȊO			�ُ�I��(simply_get_last_error�̖߂�l�Q��)
	//���ӓ_
	//	IxxatSimple�̕��́A���M�O�ɃX�e�[�^�X���m�F���APENDING��ԂɂȂ��Ă��Ȃ�����
	//	�m�F���Ă��瑗�M����K�v���L��



	//todo:CAN�C���^�[�t�F�[�X�ɑ��M���鏈�����������ĉ�����

	//�߂�l
	int32_t nResult = -1;

	//�r������J�n
	if(!LockCanMsg())
		return(-1);	//������L���s

	do
		{
		//For debug
		if(DEBUGPRINT)
			debugprint(true,nCanID,pData8);

		//�X�e�[�^�X�擾���đ��M�\�ɂȂ閘�҂�
		can_sts_t status;
		while(-1)
			{
			if(!simply_can_status(&status))
				{
				nResult = (int32_t)simply_get_last_error();
				break;
				}
			else if(status.sts & CAN_STATUS_PENDING)
				Sleep(1);
			else
				{
				nResult = 0;
				break;	//���M�\
				}
			}
		if(nResult)
			break;

		//���M
		can_msg_t msg;
		msg.dlc = nLength;
		msg.ident = nCanID | 0x80000000;
		memcpy(msg.payload,pData8,sizeof(char) * 8);
		if(!simply_send(&msg))
			{
			//���M���s
			nResult = (int32_t)simply_get_last_error();
			break;
			}
		//
		nResult = 0;
		break;
		} while(-1);

	//
	UnlockCanMsg();
	//
	return(nResult);
	}

//CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ�ꍇ�ɌĂяo����܂�
int32_t CIxxatSimple::OnCanGetError()
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�
	//���ӓ_
	//	���̃f�o�C�X�ł͔��������G���[���S�Ď�M�f�[�^�Ƃ��Ēʒm�����

	//todo:CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ鏈�����������ĉ�����

	//�����Ɋi�[�����G���[���L�邩�H
	if(m_status.err.info.nErrorStatus)
		return(1);

	return(0);
	}

//CAN�C���^�[�t�F�[�X�̃G���[����������ꍇ�ɌĂяo����܂�
int32_t CIxxatSimple::OnCanClearError()
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃G���[���������܂�
	//	�����ۑ��̃G���[�R�[�h���������邾���ŁA�n�[�h�E�F�A�̃G���[�͉����s��
	//	�i�n�[�h�E�F�A�G���[�́A��x���S�ɕ��Ȃ���(simply_close)�����o���Ȃ��j
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:CAN�C���^�[�t�F�[�X�̃G���[���������鏈�����������ĉ�����

	//�����Ɋi�[�����G���[�l������
	m_status.err.nRaw = 0;

	return(0);
	}

//��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
int32_t CIxxatSimple::OnCanRecvClear()
	{
	//�T�v
	//	��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�
	//���ӓ_
	//	���Ɏ�����L���ꂽ��ԂŌĂяo�����̂ŁA������L���Ȃ���


	//todo:��M�o�b�t�@���N���A���鏈�����������ĉ�����

	//��M�o�b�t�@�r������v��
	if(LockBuffer())
		{
		//��M�o�b�t�@�ʒu��������
		m_nCanReadPt = 0;
		m_nCanWritePt = 0;
		//��M�o�b�t�@�r���������
		UnlockBuffer();
		}
	//
	return(0);
	}

//================================================================================
//�{�N���X��p�̗v�f
//================================================================================

//�񓯊���M�X���b�h
unsigned int __stdcall CIxxatSimple::ReceiveThread(void* Param)
	{
	//����T�v
	//	��M�����ꍇ
	//		�L���ȃf�[�^�̏ꍇ
	//			��M�f�[�^��o�^���A�҂����ԂȂ��ōēx��M�ɖ߂�
	//		�����ȃf�[�^�̏ꍇ
	//			�҂����ԂȂ��ōēx��M�ɖ߂�
	//	��M���Ȃ������A���̓G���[���o���ꍇ
	//		��莞�ԑ҂��āA��M�ɖ߂�

	//�Ăяo�����N���X�ւ̃|�C���^
	CIxxatSimple* pClass = (CIxxatSimple*)Param;

	//
	DWORD		nWaitTimeoutMS = 10;	//10[ms]
	uint32_t	nStage = 0;				//��M�҂��X�e�[�W����J�n
	can_msg_t	canMsg;

	//
	while (!pClass->m_bQuitThread)
		{
		//��M�X�e�[�W
		if (nStage == 0)
			{
			//�r������
			if(pClass->LockCanMsg())
				{
				//��M
				int8_t nResult = simply_receive(&canMsg);
				//���b�Z�[�W��M�H
				if(nResult == 1)
					{
					//���ꃁ�b�Z�[�W�H
					if(canMsg.dlc & 0x80)
						{
						//�o�^���Ȃ�
						}
					else
						{
						//ID�ɍ׍H
						canMsg.ident &= 0x7fffffff;
						//�o�^
						pClass->AddCanMsg(&canMsg,1);
						}
					}
				//�G���[���͎�M�����H
				else
					nStage = 1; 
				//�r������J��
				pClass->UnlockCanMsg();
				}
			else
				{
				//�r�����䎸�s�͎��ԑ҂��ɑJ��
				nStage = 1;
				}
			}
		//���ԑ҂��X�e�[�W
		else if (nStage == 1)
			{
			//��莞�ԑ҂�
			Sleep(1);
			//��M�X�e�[�W�ɑJ��
			nStage = 0;
			}
		}
	//
	_endthreadex(0);
	return(0);
	}

//�r������v��
bool CIxxatSimple::LockCanMsg(DWORD nTimeoutMS /* INFINITE */)
	{
	//��L�J�n
	if(::WaitForSingleObject(m_hSema,nTimeoutMS) != WAIT_OBJECT_0)
		return(false);	//�m�ێ��s
	//�m�ې���
	return(true);
	}

//�r���������
void CIxxatSimple::UnlockCanMsg()
	{
	//��L�J��
	::ReleaseSemaphore(m_hSema,1,NULL);
	//���̃X���b�h�ɐ����n���i�҂��Ă���X���b�h���L��΁j
	Sleep(0);
	}

//��M�o�b�t�@�r������v��
bool CIxxatSimple::LockBuffer()
	{
	//��L�J�n
	if(::WaitForSingleObject(m_hSema2,INFINITE) != WAIT_OBJECT_0)
		return(false);	//�m�ێ��s
	//�m�ې���
	return(true);
	}

//��M�o�b�t�@�r���������
void CIxxatSimple::UnlockBuffer()
	{
	//
	//��L�J��
	::ReleaseSemaphore(m_hSema2,1,NULL);
	//���̃X���b�h�ɐ����n���i�҂��Ă���X���b�h���L��΁j
	Sleep(0);
	}



//��M�f�[�^�̓o�^����
uint32_t CIxxatSimple::AddCanMsg(can_msg_t* pMsg,int nCount /* 1 */)
	{
	//���ӓ_
	//	���̊֐����Ŏg�p���Ă���r������́A�o�b�t�@�̕��Ȃ̂Œ���

	//��M�o�b�t�@�r������v��
	if(!LockBuffer())
		return(-1);

	uint32_t nResult = 0;
	while(nCount)
		{
		//For DEBUG
		if(DEBUGPRINT)
			debugprint(false,pMsg->ident,pMsg->payload);

		//���̃p�P�b�g�͖������Ȃ��Ώۂ��H
		if(!IsIgnoreID(pMsg->ident))
			{
			//�o�b�t�@FULL�ł͖����H
			if(m_nCanWritePt < 255)
				{
				//�i�[
				::memcpy(&m_pCanMsg[m_nCanWritePt],pMsg,sizeof(can_msg_t));
				//�i�[��X�V
				++m_nCanWritePt;
				//�i�[���X�V
				++nResult;
				}
			else
				{
				//�o�b�t�@����t
				}
			}
		else
			{
			//��������Ώ�
			}

		//�\�[�X�̎��ʒu
		++pMsg;

		//�c��X�V
		--nCount;
		}

	//��M�o�b�t�@�r���������
	UnlockBuffer();

	//
	return(nResult);
	}

//�o�^���ꂽCAN���b�Z�[�W��1�擾
uint32_t CIxxatSimple::GetCanMsg(can_msg_t* pMsg)
	{
	//��M�o�b�t�@�r������v��
	if(!LockBuffer())
		return(2);	//������L���s���́A���̑��G���[�Ƃ��Ė߂�

	//
	uint32_t nResult = 0;

	//���ǂݏo���̗v�f���L��H
	if(m_nCanReadPt < m_nCanWritePt)
		{
		//�Y���ӏ�����擾
		::memcpy(pMsg,&m_pCanMsg[m_nCanReadPt],sizeof(can_msg_t));
		//�ǂݏo���ʒu�̍X�V
		++m_nCanReadPt;
		//�ǂݏo���c�肪�����H
		if(m_nCanReadPt >= m_nCanWritePt)
			{
			//�������i�S�N���A�����Ńo�b�t�@��擪�ɖ߂����A�r������ς݂Ȃ̂�OnCanRecvClear���g��Ȃ����j
			m_nCanReadPt = 0;
			m_nCanWritePt = 0;
			}
		nResult = 0;
		}
	else
		nResult = 1;

	//��M�o�b�t�@�r���������
	UnlockBuffer();
	//
	return(nResult);
	}
