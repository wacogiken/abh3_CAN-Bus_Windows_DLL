//========================================
//�T�v
//========================================
//	Ixxat V2�̃f�o�C�X���g���ׂ̃N���X
//
//========================================
//�����ɕK�v�ȑO����
//========================================
//	IxxatSimple�p�̃h���C�o���_�E�����[�h���ēW�J�iSDK�������Ă���j
//	�v���W�F�N�g�ݒ�ŁA�ǉ��C���N���[�h�t�H���_�Ɉȉ���ǉ�
//		"<�W�J�����t�H���_>\inc"
//	�v���W�F�N�g�ݒ�ŁA�ǉ����C�u�����t�H���_�Ɉȉ���ǉ�
//		"<�W�J�����t�H���_>\lib\dll"
//	�v���W�F�N�g�̎��s�t�@�C�����i�[�����ꏊ�ɁA��L���������t�H���_����DLL���R�s�[
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
	m_hSema = NULL;
	m_hReadThread = NULL;
	m_bQuitThread = false;
	m_nCanReadPt = 0;
	m_nCanWritePt = 0;
	m_pCanMsg = NULL;
	}

//�f�X�g���N�^
CIxxatSimple::~CIxxatSimple()
	{
	//������J���Ă���ꍇ���L��̂ŕ��鏈�����s��
	OnCloseInterface();
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
	//	0�ȊO			�ُ�I��

	//todo:CAN�C���^�[�t�F�[�X���J���������������ĉ�����

	//�J���Ă���ꍇ���L��̂ŕ���
	OnCloseInterface();

	//�f�o�C�X�����\�z���ĉ�����J��
	char* pDeviceName = new char[16]();
	sprintf_s(pDeviceName,16,"COM%d",nDeviceNum);
	bool bResult = simply_open(pDeviceName);
	delete pDeviceName;
	if(!bResult)
		return(-1);

	//����̏�����
	if(!simply_initialize_can(uint16_t(GetBaudrate())))
		{
		//���s
		simply_close();
		return(-1);	//Error;
		}

	//CAN�̊J�n
	else if(!simply_start_can())
		{
		//���s
		simply_reset_can();
		simply_close();
		return(-1);	//
		}

	//�X�e�[�^�X�̏�����
	m_status.err.nRaw = 0;

	//��M�f�[�^�r������p�Z�}�t�H
	m_hSema = ::CreateSemaphore(NULL,1,1,NULL);

	//��M�f�[�^�i�[�p�̈�̏�����
	m_pCanMsg = new can_msg_t[256]();
	m_nCanReadPt = 0;
	m_nCanWritePt = 0;

	//��M�X���b�h�J�n
	m_bQuitThread = false;
	::SetThreadPriority(m_hReadThread = (HANDLE)_beginthreadex(NULL,0,&CIxxatSimple::ReceiveThread,this,NULL,&m_nThreadID),THREAD_PRIORITY_NORMAL);

	//�ڑ��ς݃t���O�ݒ�
	m_status.bOpen = true;

	//��������
	return(0);
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

	//��M�o�b�t�@�r������p�Z�}�t�H�J��
	if(m_hSema)
		{
		::CloseHandle(m_hSema);
		m_hSema = NULL;
		}

	//��M�o�b�t�@�J��
	SafeDelete(&m_pCanMsg);
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
	//	0�ȊO			�ُ�I��

	//todo:CAN�C���^�[�t�F�[�X����1�񕪂̎�M���s���������������ĉ�����

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
	//	0�ȊO			�ُ�I��

	//todo:CAN�C���^�[�t�F�[�X�ɑ��M���鏈�����������ĉ�����

	//For debug
	if(DEBUGPRINT)
		debugprint(true,nCanID,pData8);

	//
	can_msg_t msg;
	msg.dlc = nLength;
	msg.ident = nCanID | 0x80000000;
	memcpy(msg.payload,pData8,sizeof(char) * 8);

	if(!simply_send(&msg))
		return(-1);	//Error
	return(0);
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
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:CAN�C���^�[�t�F�[�X�̃G���[���������鏈�����������ĉ�����

	//�r�����䐬���H
	if(LockCanMsg())
		{
		//�����Ɋi�[�����G���[�l������
		m_status.err.nRaw = 0;
		//�r���������
		UnlockCanMsg();
		}
	//�f�o�C�X�̃G���[������
	//simply_reset_can();

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

	//todo:��M�o�b�t�@���N���A���鏈�����������ĉ�����

	//�r�����䐬���H
	if(LockCanMsg())
		{
		//��M�o�b�t�@�ʒu��������
		m_nCanReadPt = 0;
		m_nCanWritePt = 0;
		//�r���������
		UnlockCanMsg();
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
	//�Ăяo�����N���X�ւ̃|�C���^
	CIxxatSimple* pClass = (CIxxatSimple*)Param;

	//
	DWORD		nWaitTimeoutMS = 10;	//10[ms]
	uint32_t	nStage = 0;				//��M�҂��X�e�[�W����J�n
	can_msg_t	canMsg;

	//
	while (!pClass->m_bQuitThread)
		{
		//��M
		if (nStage == 0)
			{
			//��M
			int8_t nResult = simply_receive(&canMsg);
			//��M�����H
			if(nResult == 0)
				nStage = 1; 
			//���b�Z�[�W��M�H
			else if(nResult == 1)
				nStage = 2;
			//�G���[�H
			else
				nStage = 90;
			}
		//���ԑ҂�
		else if (nStage == 1)
			{
			//��莞�ԑ҂�
			Sleep(1);
			//��M�X�e�[�W�ɑJ��
			nStage = 0;
			}
		//��M�f�[�^�̓o�^
		else if(nStage == 2)
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
			//��M�X�e�[�W�ɑJ��
			nStage = 0;
			}
		//�G���[
		else
			{
			can_sts_t status;
			if(simply_can_status(&status))
				{
				//�X�e�[�^�X���擾�����̂œo�^
				pClass->m_status.err.nRaw = status.sts;
				}
			else
				{
				//�X�e�[�^�X�擾�G���[
				pClass->m_status.err.info.nErrorStatus = 1;	//�G���[����������Ȃ��̂ŁA�����I�ɃG���[����
				}
			//�����҂�����
			Sleep(100);
			//��M�X�e�[�W�ɑJ��
			nStage = 0;
			}
		}
	//
	_endthreadex(0);
	return(0);
	}

//��M�o�b�t�@�r������v��
bool CIxxatSimple::LockCanMsg(DWORD nTimeoutMS /* INFINITE */)
	{
	if(::WaitForSingleObject(m_hSema,nTimeoutMS) != WAIT_OBJECT_0)
		return(false);	//�m�ێ��s
	//�m�ې���
	return(true);
	}

//��M�o�b�t�@�r���������
void CIxxatSimple::UnlockCanMsg()
	{
	::ReleaseSemaphore(m_hSema,1,NULL);
	}

//��M�f�[�^�̓o�^����
uint32_t CIxxatSimple::AddCanMsg(can_msg_t* pMsg,int nCount /* 1 */)
	{
	//������L
	if(!LockCanMsg())
		return(0);	//������L���s���́A�o�^��0�Ƃ��Ė߂�

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
	//�����J��
	UnlockCanMsg();
	//
	return(nResult);
	}

//�o�^���ꂽCAN���b�Z�[�W��1�擾
uint32_t CIxxatSimple::GetCanMsg(can_msg_t* pMsg)
	{
	//
	uint32_t nResult = 0;
	//������L
	if(!LockCanMsg())
		return(2);	//������L���s���́A���̑��G���[�Ƃ��Ė߂�
	//���ǂݏo���̗v�f���L��H
	else if(m_nCanReadPt < m_nCanWritePt)
		{
		//�Y���ӏ�����擾
		::memcpy(pMsg,&m_pCanMsg[m_nCanReadPt],sizeof(can_msg_t));
		//�ǂݏo���ʒu�̍X�V
		++m_nCanReadPt;
		//�ǂݏo���c�肪�����H
		if(m_nCanReadPt >= m_nCanWritePt)
			{
			//�������i�S�N���A�����Ńo�b�t�@��擪�ɖ߂����A�r������ς݂Ȃ̂�ClearCanMsg���g��Ȃ����j
			m_nCanReadPt = 0;
			m_nCanWritePt = 0;
			}
		nResult = 0;
		}
	else
		nResult = 1;

	//�����J��
	UnlockCanMsg();
	//
	return(nResult);
	}
