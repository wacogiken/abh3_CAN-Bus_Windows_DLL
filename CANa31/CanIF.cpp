
//�T�v
//	CAN�p�f�o�C�X�N���X
//	�iCAN�C���^�[�t�F�[�X�̎������s���N���X�̌p�����j
//
//�g�p���@
//	�{�N���X���p�������N���X���\�z���A���z�֐��S�Ă��I�[�o���C�h���A��������������
//	CAN����N���X(CAbh3)���̃R���X�g���N�^�ɁA�p�������N���X��n��
//	�i�p�������N���X�́ACAN����N���X�̔j�������܂ŁA���݂��Ă���K�v���L��j
//

#include "stdafx.h"
#include "CanIF.h"
#include <timeapi.h>	//Windows.h����ɂȂ�l�ɃC���N���[�h����

//TimeAPI(GetTM�֐��ׂ̈ɕK�v)
#pragma comment(lib,"winmm.lib")

//���p�\��CAN�C���^�[�t�F�[�X�����擾����ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnGetInterfaceCount()
	{
	//�T�v
	//	�w��CAN�C���^�[�t�F�[�X�̗��p�\�����擾����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	���p�\�C���^�[�t�F�[�X��

	//todo:CAN�C���^�[�t�F�[�X�̐���߂��������������ĉ�����

	return(0);
	}

//�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnOpenInterface(int32_t nDeviceNum)
	{
	//�T�v
	//	�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	nDeviceNum		0�J�n�̃C���^�[�t�F�[�X�ԍ�
	//�߂�l
	//	0				����I��
	//	0�ȊO			�ُ�I��
	//���ӓ_
	//	�^�C���A�E�g���Ԃ́AGetTimeout,GetTimeout_Open�֐��Ŏ擾�o���܂�

	//todo:CAN�C���^�[�t�F�[�X���J���������������ĉ�����

	return(-1);
	}

//�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
void CCanIF::OnCloseInterface()
	{
	//�T�v
	//	�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	����

	//todo:CAN�C���^�[�t�F�[�X���J���Ă���ꍇ�ɕ��鏈�����������ĉ�����

	}

//CAN�C���^�[�t�F�[�X�����M����ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanRecv(uint32_t* pCanID,uint8_t* pData8)
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

	return(-1);
	}

//CAN�C���^�[�t�F�[�X�ɑ��M����ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
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

	return(-1);
	}

//CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ�ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanGetError()
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ鏈�����������ĉ�����

	return(-1);
	}

//CAN�C���^�[�t�F�[�X�̃G���[����������ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanClearError()
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃G���[���������܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:CAN�C���^�[�t�F�[�X�̃G���[���������鏈�����������ĉ�����

	return(-1);
	}

//��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
int32_t CCanIF::OnCanRecvClear()
	{
	//�T�v
	//	��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
	//�p�����[�^
	//	����
	//�߂�l
	//	0				����
	//	0�ȊO			�ُ�

	//todo:��M�o�b�t�@���N���A���鏈�����������ĉ�����

	return(-1);
	}

//================================================================================
//��ʂ���̐ݒ���󂯂�֐�
//================================================================================

//CAN�C���^�[�t�F�[�X�̃^�C���A�E�g��ݒ�
void CCanIF::SetTimeout(uint32_t nOpenMS,uint32_t nSendMS,uint32_t nRecvMS)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃^�C���A�E�g���ꊇ�ݒ�
	//�p�����[�^
	//	nOpenMS		������J�����̃^�C���A�E�g[ms]
	//	nSendMS		���M���̃^�C���A�E�g[ms]
	//	nRecvMS		��M���̃^�C���A�E�g[ms]
	//�߂�l
	//	����

	m_var.timeout.nOpen = nOpenMS;
	m_var.timeout.nSend = nSendMS;
	m_var.timeout.nRecv = nRecvMS;
	}
void CCanIF::SetTimeout_Open(uint32_t nTimeoutMS)
	{
	m_var.timeout.nOpen = nTimeoutMS;
	}
void CCanIF::SetTimeout_Send(uint32_t nTimeoutMS)
	{
	m_var.timeout.nSend = nTimeoutMS;
	}
void CCanIF::SetTimeout_Recv(uint32_t nTimeoutMS)
	{
	m_var.timeout.nRecv = nTimeoutMS;
	}

//���݂�CAN�C���^�[�t�F�[�X�^�C���A�E�g�ݒ���擾
void CCanIF::GetTimeout(uint32_t& nOpenMS,uint32_t& nSendMS,uint32_t& nRecvMS)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃^�C���A�E�g���ꊇ�擾
	//�p�����[�^
	//	nOpenMS		������J�����̃^�C���A�E�g[ms]���i�[����Q��
	//	nSendMS		���M���̃^�C���A�E�g[ms]���i�[����Q��
	//	nRecvMS		��M���̃^�C���A�E�g[ms]���i�[����Q��
	//�߂�l
	//	����
	nOpenMS = m_var.timeout.nOpen;
	nSendMS = m_var.timeout.nSend;
	nRecvMS = m_var.timeout.nRecv;
	}
uint32_t CCanIF::GetTimeout_Open(void)
	{
	return(m_var.timeout.nOpen);
	}
uint32_t CCanIF::GetTimeout_Send(void)
	{
	return(m_var.timeout.nSend);
	}
uint32_t CCanIF::GetTimeout_Recv(void)
	{
	return(m_var.timeout.nRecv);
	}

//CAN�C���^�[�t�F�[�X�̃{�[���[�g��ݒ�
void CCanIF::SetBaudrate(uint32_t nBaudrate)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�̃{�[���[�g��ݒ�
	//�p�����[�^
	//	nBaudrate		�f�o�C�X�ˑ��̃{�[���[�g���w�肷��l
	//�߂�l
	//	����
	//���ӓ_
	//	�ݒ�l�̒P�ʓ��́A�f�o�C�X�Ɉˑ�����
	//	
	m_var.nBaudrate = nBaudrate;
	}

//���݂�CAN�C���^�[�t�F�[�X�̃{�[���[�g���擾
uint32_t CCanIF::GetBaudrate(void)
	{
	return(m_var.nBaudrate);
	}

//�������锭�M��ID���w��
void CCanIF::SetIgnoreID(uint8_t nID)
	{
	m_var.nIgnoreID = nID;
	}

//�������锭�M��ID���擾
uint8_t CCanIF::GetIgnoreID(uint8_t nID)
	{
	return(m_var.nIgnoreID);
	}

//�B��̎�M�Ώ�ID��ݒ�
void CCanIF::SetRecvOnlyID(uint32_t nID)
	{
	//�p�����[�^
	//	nID		�B��̎�M�ΏۂƂȂ�ID�i0�w��ŉ����j
	m_var.nOnlyID = nID;
	}

//�ݒ�ςݗB��̎�M�Ώ�ID���擾
uint32_t CCanIF::GetRecvOnlyID()
	{
	return(m_var.nOnlyID);
	}

//================================================================================
//��ʊ܂߂ė��p�\�Ȋ֐�
//================================================================================

//���ݎ�����[ms]�P�ʂŎ擾
uint32_t CCanIF::GetTm()
	{
	MMTIME x;
	timeGetSystemTime(&x,sizeof(MMTIME));
	return(uint32_t(x.u.ticks));
	}
