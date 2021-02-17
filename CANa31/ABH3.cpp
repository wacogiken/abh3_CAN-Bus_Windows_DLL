//�T�v
//	ABH3�pCAN����N���X
//
//����
//	2020/09/xx	yo0043		1st release
//	2020/09/29	yo0043		2nd release
//							���M����8�o�C�g�����ɂ���K�v���L��ׁA����̊֐��ɑ��M����ǉ�
//							�}���`�p�P�b�g�̃f�t�H���g�v������ 0xff -> 0x08 �ɕύX
//							�}���`�p�P�b�g��r���Œ��~����ꍇ��ABORT�𔭍s����l�ɏC��
//							�i���M�Ɏ��s�����ꍇ����Ȍ����ׁ̈AABORT�����ۂɑ��M����邩�͎��̉^�j
//	2021/02/15	yo0043		3rd release
//							�R�����g�ǉ��݂̂Ńv���O�����ɕύX����
//

#include "stdafx.h"
#include <memory.h>		//memset,
#include "Abh3.h"

//�R���X�g���N�^
CAbh3::CAbh3(CCanIF* pDeviceClass /* NULL */)
	{
	//�����ϐ��̏�����
	::memset(&m_var,0,sizeof(CANABH3_VAR));

	//�ʐM����p�Z�}�t�H�\�z
	m_var.hTerm = ::CreateSemaphore(NULL,1,1,NULL);

	//�o�͒i�̃N���X��ۑ�
	m_var.pDeviceClass = pDeviceClass;
	}

//�f�X�g���N�^
CAbh3::~CAbh3()
	{
	//�C���^�[�t�F�[�X�����i�J���Ă���ꍇ���L��ׁj
	CloseInterface();

	//�ʐM����p�Z�}�t�H�̊J��
	::CloseHandle(m_var.hTerm);
	m_var.hTerm = NULL;
	}

//================================================================================
//���[�U�[�����p�\�Ȋ֐��i��{�v�f�j
//================================================================================

//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
int32_t CAbh3::GetInterfaceCount(void)
	{
	//�f�o�C�X�����o�^�H
	if(!IsValidDevice())
		return(0);	//�f�o�C�X��������
	return(m_var.pDeviceClass->OnGetInterfaceCount());
	}

//�w��C���^�[�t�F�[�X���J��
int32_t CAbh3::OpenInterface(int32_t nDeviceNum)
	{
	//CAN�C���^�[�t�F�[�X���J���Ă���ꍇ���L��̂ŕ���
	CloseInterface();

	//�f�o�C�X�����o�^�H
	if(!IsValidDevice())
		return(-1);	//�G���[

	//CAN�C���^�[�t�F�[�X���J��
	int32_t nResult = m_var.pDeviceClass->OnOpenInterface(nDeviceNum);
	//����I���H
	if(nResult == 0)
		{
		//�J����CAN�C���^�[�t�F�[�X����ۑ�
		m_var.nOpenDevice = nDeviceNum + 1;
		//�f�o�C�X�Ƀz�X�g��ID��n���āA��������ΏۂƂ�����
		m_var.pDeviceClass->SetIgnoreID(GetHostID());
		}
	return(nResult);
	} 

//�J�����C���^�[�t�F�[�X�����
void CAbh3::CloseInterface()
	{
	//�f�o�C�X�o�^�L��H
	if(IsValidDevice())
		{
		//CAN�C���^�[�t�F�[�X�����
		m_var.pDeviceClass->OnCloseInterface();
		}
	//�J���Ă���CAN�C���^�[�t�F�[�X��������
	m_var.nOpenDevice = 0;
	}

//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
int32_t CAbh3::GetCurrentInterface()
	{
	return(m_var.nOpenDevice - 1);
	}

//���݃C���^�[�t�F�[�X���J���Ă��邩�H
int32_t CAbh3::IsOpenInterface()
	{
	if(GetCurrentInterface() >= 0)
		return(1);
	return(0);
	}

//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
void CAbh3::SetOpenTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_var.pDeviceClass->SetTimeout_Open(nTimeoutMS);
	}

//���M�^�C���A�E�g���Ԃ�ݒ�
void CAbh3::SetSendTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_var.pDeviceClass->SetTimeout_Send(nTimeoutMS);
	}

//��M�^�C���A�E�g���Ԃ�ݒ�
void CAbh3::SetRecvTimeout(uint32_t nTimeoutMS)
	{
	if(IsValidDevice())
		m_var.pDeviceClass->SetTimeout_Recv(nTimeoutMS);
	}

//�ʐM�Ώ�ABH3�̃A�h���X��ݒ�
void CAbh3::SetTargetID(uint8_t nAdrs)
	{
	m_var.config.nTargetAdrs = nAdrs;
	}

//�ʐM�Ώ�ABH3�̃A�h���X���擾
uint8_t CAbh3::GetTargetID()
	{
	return(m_var.config.nTargetAdrs);
	}

//�ʐM�z�X�g�̃A�h���X��ݒ�
void CAbh3::SetHostID(uint8_t nAdrs)
	{
	m_var.config.nHostAdrs = nAdrs;
	}

//�ʐM�z�X�g�̃A�h���X���擾
uint8_t CAbh3::GetHostID()
	{
	return(m_var.config.nHostAdrs);
	}

//�ʐM���x���w��
void CAbh3::SetBaudrate(uint32_t nBaudrateKbps)
	{
	//�T�v
	//	�ʐM�{�[���[�g�w��
	//�p�����[�^
	//	nBaudrateKbps	�{�[���[�g��[kbps]�P�ʂŎw��i�f�o�C�X�ˑ��Ȃ̂Œ��Ӂj

	//�f�o�C�X���o�^�ς݂Ȃ�{�[���[�g�ݒ�
	if(IsValidDevice())
		m_var.pDeviceClass->SetBaudrate(nBaudrateKbps);
	}

//�ݒ肵���ʐM���x���擾
uint32_t CAbh3::GetBaudrate(void)
	{
	//�f�o�C�X�����o�^�H
	if(!IsValidDevice())
		return(0);
	return(m_var.pDeviceClass->GetBaudrate());
	}

//================================================================================
//���[�U�[�����p�\�Ȋ֐��iABH3�v�f�j
//================================================================================

//�w�߂̏�����
int32_t CAbh3::abh3_can_init()
	{
	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�ݒ�l���i�[
			m_var.lastdata.send.nOrderAY = 0;
			m_var.lastdata.send.nOrderBX = 0;
			m_var.lastdata.send.nInputBit = 0;

			//�V���O���p�P�b�g(DP0)���\�z
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 m_var.lastdata.send.nOrderAY
				,m_var.lastdata.send.nOrderBX
				,m_var.lastdata.send.nInputBit
				);

			//���M��ID���쐬
			uint32_t nSendID = CreateID(0x00ef0000);

			//��M�p�o�b�t�@�i�g��Ȃ��j
			CANABH3_RESULT result;

			//��M�o�b�t�@�̃N���A
			ClearRecvBuffer();

			//���M
			nResult = CanTermSingle(nSendID,pPacket,&result,8);

			//�J��
			CCan1939::FreeBuffer(pPacket);
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//����
	return(nResult);
	}

//�w�߂̑��M�i���ʁj
int32_t CAbh3::abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr)
	{
	//abh3_can_cmd�𗘗p����

	//���s
	return(abh3_can_cmd(cmd,m_var.lastdata.send.nOrderBX,pPtr));
	}
int32_t CAbh3::abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr)
	{
	//abh3_can_cmd�𗘗p����

	//���s
	return(abh3_can_cmd(m_var.lastdata.send.nOrderAY,cmd,pPtr));
	}

//�w�߂̑��M�i�����j
int32_t CAbh3::abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr)
	{
	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�ݒ�l���i�[
			m_var.lastdata.send.nOrderAY = cmdAY;
			m_var.lastdata.send.nOrderBX = cmdBX;

			//�V���O���p�P�b�g(DP0)���\�z
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 m_var.lastdata.send.nOrderAY
				,m_var.lastdata.send.nOrderBX
				,m_var.lastdata.send.nInputBit
				);

			//���M��ID���쐬
			uint32_t nSendID = CreateID(0x00ef0000);

			//��M�o�b�t�@�̃N���A
			ClearRecvBuffer();

			//���M
			nResult = CanTermSingle(nSendID,pPacket,pPtr,8);

			//�J��
			CCan1939::FreeBuffer(pPacket);
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//�\���̂ɑ��M�v�f����������
	SetLastData(pPtr);

	//����
	return(nResult);
	}

//���͂̑��M�i�ꊇ�j
int32_t CAbh3::abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr)
	{
	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�ݒ�l���i�[
			m_var.lastdata.send.nInputBit = (data & ~mask) | (data & mask);

			//�V���O���p�P�b�g(DP0)���\�z
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 m_var.lastdata.send.nOrderAY
				,m_var.lastdata.send.nOrderBX
				,m_var.lastdata.send.nInputBit
				);

			//���M��ID���쐬
			uint32_t nSendID = CreateID(0x00ef0000);

			//��M�o�b�t�@�̃N���A
			ClearRecvBuffer();

			//���M
			nResult = CanTermSingle(nSendID,pPacket,pPtr,8);

			//�J��
			CCan1939::FreeBuffer(pPacket);
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//�\���̂ɑ��M�v�f����������
	SetLastData(pPtr);

	//����
	return(nResult);
	}

//���͂̑��M�i�r�b�g�j
int32_t CAbh3::abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr)
	{
	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�ݒ�l���i�[
			if(data)
				m_var.lastdata.send.nInputBit |= (1 << num);
			else
				m_var.lastdata.send.nInputBit &= ~(1 << num);

			//�V���O���p�P�b�g(DP0)���\�z
			uint8_t* pPacket = CCan1939::CreateSGL0(
				 m_var.lastdata.send.nOrderAY
				,m_var.lastdata.send.nOrderBX
				,m_var.lastdata.send.nInputBit
				);

			//���M��ID���쐬
			uint32_t nSendID = CreateID(0x00ef0000);

			//��M�o�b�t�@�̃N���A
			ClearRecvBuffer();

			//���M
			nResult = CanTermSingle(nSendID,pPacket,pPtr,8);

			//�J��
			CCan1939::FreeBuffer(pPacket);
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//�\���̂ɑ��M�v�f����������
	SetLastData(pPtr);

	//����
	return(nResult);
	}

//�ώZ�l�̃��N�G�X�g
int32_t CAbh3::abh3_can_reqPulse(pCANABH3_RESULT pPtr)
	{
	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�V���O���p�P�b�g(DP1)���\�z
			uint8_t* pPacket = CCan1939::CreateSGL1();

			//���M��ID���쐬
			uint32_t nSendID = CreateID(0x01ef0000);

			//��M�o�b�t�@�̃N���A
			ClearRecvBuffer();

			//���M
			nResult = CanTermSingle(nSendID,pPacket,pPtr,0);

			//�J��
			CCan1939::FreeBuffer(pPacket);
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//�\���̂ɑ��M�v�f����������
	SetLastData(pPtr);

	//����
	return(nResult);
	}

//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
int32_t CAbh3::abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr)
	{
	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�V�O�l�`�����\�z
			uint32_t nSign = CreateBrdID(num);

			//�u���[�h�L���X�g�p�P�b�g(num)���\�z
			uint8_t* pPacket = CCan1939::CreateBRD(nSign);

			//���M��ID���쐬
			uint32_t nSendID = CreateID(0x00ea0000);

			//��M�o�b�t�@�̃N���A
			ClearRecvBuffer();

			//��M�Ώۂ�ID���\�z
			//	�i���M�悩�甭�M����A�w��ԍ�����M��ɂȂ��Ă��镨���Ώہj
			uint32_t nOnlyID = 0x00ff0000 | (num << 8) | GetTargetID();

			//���M
			nResult = CanTermSingle(nSendID,nOnlyID,pPacket,pPtr,3);

			//�J��
			CCan1939::FreeBuffer(pPacket);
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//�\���̂ɑ��M�v�f����������
	SetLastData(pPtr);

	//����
	return(nResult);
	}

//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
int32_t CAbh3::abh3_can_trans(char* sbuf,char*& rbuf,size_t& rbuflen)
	{
	//�T�v
	//	sbuf		���M�f�[�^���i�[���ꂽ�|�C���^
	//	rbuf		��M�f�[�^���i�[����|�C���^�ϐ��ւ̎Q��
	//	rbuflen		��M�f�[�^�o�b�t�@��
	//�߂�l
	//	0			����
	//	0�ȊO		�ُ�
	//���ӓ_
	//	rbuf�͌Ăяo�����ŊJ��(delete)����K�v������܂�
	//	�ʐM���������͎��s����܂ŏ������߂�܂���
	//�g�p��
	//	char* sbuf = "CP000095";
	//	char* rbuf = NULL;
	//	size_t rbuflen = 0;
	//	if(abh3_can_trans(sbuf,rbuf,rbuflen) == 0)
	//		{
	//		//����ȏꍇ�̏���
	//		printf("%s\n",rbuf);
	//		}
	//	else
	//		{
	//		//�ُ�ȏꍇ�̏���
	//		printf("Error\n");
	//		}
	//	if(rbuf)
	//		delete rbuf;

	//�߂�l
	int32_t nResult = -1;

	//�ʐM�r������p�̃Z�}�t�H���擾
	if(Lock() == 0)
		{
		//�C���^�[�t�F�[�X���J���Ă���H
		if(IsOpenInterface())
			{
			//�ꎞ�o�b�t�@�\�z
			//2020.10.09 yo0043 �ȉ��d�l�ɕύX
			//	�f�[�^�������Œ�9�o�C�g�ɂȂ�l��1�ȏ��SYN��ǉ�

			//������̃T�C�Y�����ɁA�o�b�t�@�T�C�Y���Z�o
			size_t nSrcLen = ::strlen(sbuf);
			size_t nBufLen = 0;
			if(nSrcLen < 9)
				nBufLen = 9;
			else
				nBufLen = nSrcLen + 1;

			//�I�[�����l�����ăo�b�t�@���\�z
			char* pTmp = new char[nBufLen + 1]();	//�������t���Ŋm�ہi�I�[�ׁ̈j

			//�I�[��������SYN(0x16)�Ŗ��߂�
			::memset(pTmp,0x16,nBufLen);

			//���������擪���當�����������R�s�[����
			::memcpy(pTmp,sbuf,nSrcLen);

			//�^�ϊ�
			uint8_t* pSrc = (uint8_t*)pTmp;
			uint8_t* pDst = 0;
			uint32_t nDstSize = 0;

			//����M
			nResult = CanTermSendMulti(pSrc,uint32_t(nBufLen),pDst,nDstSize);
			if(nResult == 0)
				{
				rbuf = (char*)pDst;
				rbuflen = (size_t)nDstSize;
				}
			//�ꎞ�o�b�t�@�J��
			delete pTmp;
			}
		//�ʐM�r������p�̃Z�}�t�H���J��
		Unlock();
		}

	//����
	return(nResult);
	}

//================================================================================
//�����p
//================================================================================

//8[bytes]����M
int32_t CAbh3::CanSend8(uint32_t nSendID,uint8_t* pSendData,uint8_t nLength)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X�ɑ΂���ID���w�肵�ăf�[�^�𑗐M
	//�p�����[�^
	//	nSendID		CAN-ID(29bit)
	//	pSendData	���M�f�[�^�ւ̃|�C���^
	//�߂�l
	//	0			����I��
	//	0�ȊO		�ُ�I��
	//���ӓ_
	//	�r������͂��̊֐��̌Ăяo�������s����

	//�߂�l
	int32_t nResult = -1;

	//�f�o�C�X�N���X���o�^�ς݂Ȃ瑗�M
	if(m_var.pDeviceClass)
		nResult = m_var.pDeviceClass->OnCanSend(nSendID,pSendData,nLength);

	//����
	return(nResult);
	}

//��M
int32_t CAbh3::CanRecv8(uint32_t* pRecvID,uint8_t* pRecvData)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X����f�[�^����M
	//�p�����[�^
	//	pRecvID		��M����CAN-ID(29bit)�i�[��ւ̃|�C���^
	//	pRecvData	��M�f�[�^�i�[��ւ̃|�C���^
	//�߂�l
	//	0			����I��
	//	0�ȊO		�ُ�I��
	//���ӓ_
	//	�r������͂��̊֐��̌Ăяo�������s����

	//�߂�l
	int32_t nResult = -1;

	//�f�o�C�X�N���X���o�^�ς݂Ȃ��M
	if(m_var.pDeviceClass)
		nResult = m_var.pDeviceClass->OnCanRecv(pRecvID,pRecvData);

	//����
	return(nResult);
	}

//�V���O���p�P�b�g����M
int32_t CAbh3::CanTermSingle(uint32_t nSendID,uint8_t* pSendData,pCANABH3_RESULT pResult,uint8_t nLength)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X���g�p���ăV���O���p�P�b�g�𑗎�M
	//�p�����[�^
	//	nSendID		���M�Ώ�ID
	//	pSendData	���M�f�[�^�ւ̃|�C���^
	//	pResult		�ԓ��f�[�^�i�[��ւ̃|�C���^
	//�߂�l
	//	0			����I��
	//	0�ȊO		�ُ�I��

	//���M
	int32_t nResult = CanSend8(nSendID,pSendData,nLength);

	//���M�����H
	if(nResult == 0)
		nResult = CanRecv8(&pResult->nID,pResult->u.raw);	//��M
	//
	return(nResult);
	}
int32_t CAbh3::CanTermSingle(uint32_t nSendID,uint32_t nOnlyID,uint8_t* pSendData,pCANABH3_RESULT pResult,uint8_t nLength)
	{
	//�T�v
	//	CAN�C���^�[�t�F�[�X���g�p���ăV���O���p�P�b�g�𑗎�M
	//�p�����[�^
	//	nSendID		���M�Ώ�ID
	//	nOnlyID		��M�ΏۂƂȂ�B���ID
	//	pSendData	���M�f�[�^�ւ̃|�C���^
	//	pResult		�ԓ��f�[�^�i�[��ւ̃|�C���^
	//�߂�l
	//	0			����I��
	//	0�ȊO		�ُ�I��

	//�f�o�C�X���g���Ȃ��H
	if(!IsValidDevice())
		return(-1);		//���M�s��

	//��M�ΏۂƂȂ�B���ID��ݒ�
	m_var.pDeviceClass->SetRecvOnlyID(nOnlyID);

	//���M
	int32_t nResult = CanSend8(nSendID,pSendData,nLength);

	//���M�����H
	if(nResult == 0)
		nResult = CanRecv8(&pResult->nID,pResult->u.raw);	//��M

	//��M�ΏۂƂȂ�B���ID������
	m_var.pDeviceClass->SetRecvOnlyID(0);

	//
	return(nResult);
	}

//�}���`�p�P�b�g���M
int32_t CAbh3::CanTermSendMulti(uint8_t* pSendData,uint32_t nSendDataSize,uint8_t*& pRecvData,uint32_t& nRecvDataSize)
	{
	//�T�v
	//	�}���`�p�P�b�g���M�iPC�����N�_�Ƃ��鑗��M�j���s��
	//�p�����[�^
	//	pSendData		���M�f�[�^�ւ̃|�C���^
	//	nSendDataSize	���M�f�[�^��[bytes]
	//	pRecvData		��M�f�[�^�������|�C���^�ւ̎Q��
	//	nRecvDataSize	��M�f�[�^�����i�[����ϐ��̎Q��
	//�߂�l
	//	0				����I��
	//	-11				�ʐM�悩�璆�~�v��(ABORT)����M
	//	-12				�ʐM�悩��Z�b�V�����d���v���i�������RTS�j����M
	//	-13				PC���ő��M���s
	//	-14				PC���Ŏ�M���s�i�^�C���A�E�g�j
	//
	//���ӓ_
	//	�֐�������ɏI�������ꍇ�ApRecvData�ɂ͎�M�f�[�^���i�[���ꂽ�̈�ւ̃|�C���^������܂����A
	//	���̃|�C���^�͌Ăяo�����ŊJ������K�v���L��܂�
	//	pSendData������SYN(0x16)��ǉ����Ă���Ăяo����
	//	(abh3_can_trans�֐����Œǉ����Ă���j

	//�߂�l
	int32_t nExitCode = 0;

	//
	uint32_t nStage = 0;	//��ԑJ�ڏ����p�X�e�[�W�ԍ�
	int32_t nResult = 0;	//�ėp�̖߂�l
	uint8_t nAbort = 0;		//Abort�ԓ����̗��R

	//�z�X�g�����猩�����M�pID�Ǝ�M����pID
	uint32_t nSendID		= 0x00ec0000 | (m_var.config.nTargetAdrs << 8) | m_var.config.nHostAdrs;
	uint32_t nSendDTID		= 0x00eb0000 | (m_var.config.nTargetAdrs << 8) | m_var.config.nHostAdrs;
	uint32_t nRecvID		= 0x00ec0000 | (m_var.config.nHostAdrs << 8) | m_var.config.nTargetAdrs;
	uint32_t nRecvDTID		= 0x00eb0000 | (m_var.config.nHostAdrs << 8) | m_var.config.nTargetAdrs;
	uint32_t nID			= 0;	//
	uint8_t nMaxPacket		= 0;	//���M���p�P�b�g��
	uint8_t nPacketNum		= 0;	//�p�P�b�g�ԍ��i1-255)
	uint8_t nTotalPacket	= 0;	//��M���̑��p�P�b�g��
	//
	while(-1)
		{
		//RTS�𑗂�X�e�[�W
		//	PC�����M�m�[�h�EABH3����M�m�[�h�ő��M�v���𑗂�
		if(nStage == 0)
			{
			//���M���p�P�b�g�����������i�z�X�g�̍ő�l�j
			nMaxPacket = 0xff;

			//CM_RTS���M
			uint8_t* pPacket = CCan1939::CreateCMRTS(nSendDataSize,nMaxPacket);
			nResult = CanSend8(nSendID,pPacket,8);
			CCan1939::FreeBuffer(pPacket);
			//���M����H
			if(nResult == 0)
				nStage = 1;
			else
				{
				//���M���s
				nExitCode = -13;
				nStage = 99;	//RTS���M���Ɏ��s�A�������M���ĂȂ��̂ł��̂܂܏I��
				}
			}

		//CTS/EOMA/ABORT���󂯎��X�e�[�W
		//	PC�����M�m�[�h�EABH3����M�m�[�h�ő��M�v��(RTS)���̓f�[�^���M(DT)�̕ԓ����߂�
		else if(nStage == 1)
			{
			uint8_t* pPacket = CCan1939::CreateBuffer();
			nResult = CanRecv8(&nID,pPacket);
			if(nResult == 0)
				{
				//��M�͐���

				//���̃p�P�b�g�H
				uint8_t nType = CCan1939::IsPacket(pPacket);

				//���M�v���H
				if(nType == CCan1939::DEF_CTS)
					{
					//�^�[�Q�b�g������w�肳�ꂽ���M���p�P�b�g����ۑ�
					nMaxPacket = pPacket[1];
					//���ɑ��M����p�P�b�g�ԍ���ۑ�
					nPacketNum = pPacket[2];
					//DT�𑗂�X�e�[�W�ɑJ��
					nStage = 2;
					}
				//�����ʒm�H
				else if(nType == CCan1939::DEF_EOMA)
					{
					//��M��������
					nStage = 3;
					}
				//���~�v���H
				else if(nType == CCan1939::DEF_ABORT)
					{
					//���~����
					nExitCode = -11;
					nStage = 99;
					}
				//�V�K�v���H
				else if(nType == CCan1939::DEF_RTS)
					{
					//�Z�b�V�����d�������ŃG���[
					nExitCode = -12;
					nAbort = 1;
					nStage = 10;
					}
				//��L�ȊO�H
				else
					{
					//��L�ȊO�͌��Ȃ��������ɂ��čđ���҂�
					Sleep(0);
					}
				}
			else
				{
				//��M�����Ɏ��s�i�^�C���A�E�g�j
				nExitCode = -14;
				nAbort = 3;	//Timeout
				nStage = 10;
				}
			//
			CCan1939::FreeBuffer(pPacket);
			}

		//DT�𑗂�X�e�[�W
		//	PC�����M�m�[�h�EABH3����M�m�[�h��1�O�̕ԓ�(CTS)�Ŏw�肳�ꂽ�f�[�^�𑗂�
		//	����ʒu(nPacketNum)�ƌ�(nMaxPacket)�́A1�O�̕ԓ�(CTS)�Ŏw�肳��Ă���
		else if(nStage == 2)
			{
			//�v������0?
			if(nMaxPacket < 1)
				{
				//�Œ�1�͑���
				nMaxPacket = 1;
				}
			//
			for(uint8_t nLoop = 0;nLoop < nMaxPacket;nLoop++)
				{
				//CM_DT�p�P�b�g���쐬�o�����瑗�M����
				uint8_t* pPacket = CCan1939::CreateCMDT(pSendData,nSendDataSize,nPacketNum);
				if(pPacket)
					nResult = CanSend8(nSendDTID,pPacket,8);
	
				//����ʒu���X�V
				++nPacketNum;

				//�쐬����CM_DT�p�P�b�g���J��
				CCan1939::FreeBuffer(pPacket);

				//���M���s�H
				if(nResult)
					break;	//�r���Ŕ�����
				}

			//CM_DT�p�P�b�g�̑��M�Ɏ��s�H
			if(nResult)
				{
				//���M���s
				nExitCode = -13;
				nAbort = 2;	//resource�s��
				nStage = 10;
				}
			else
				nStage = 1;
			}

		//EOMA���󂯎�������RTS��M�X�e�[�W
		//	PC����M�m�[�h�EABH3�����M�m�[�h�ɖ����ύX���A�ԓ�(CM_RTS)���͂��͂��Ȃ̂ł������M
		else if(nStage == 3)
			{
			//��M�̈�\�z
			uint8_t* pPacket = CCan1939::CreateBuffer();
			nResult = CanRecv8(&nID,pPacket);
			//��M�͐���H
			if(nResult == 0)
				{
				//���̃p�P�b�g�H
				uint8_t nType = CCan1939::IsPacket(pPacket);
				//CM_RTS?
				if(nType == CCan1939::DEF_RTS)
					{
					//���b�Z�[�W�T�C�Y���擾���A�o�b�t�@���\�z
					nRecvDataSize = CCan1939::Get16L(pPacket,1);
					pRecvData = new uint8_t[nRecvDataSize + 1]();	//�I�[�p��+1���ď������ς݂Ŋm�ۂ���

					//�^�[�Q�b�g������w�肳�ꂽ���p�P�b�g����ۑ�
					nTotalPacket = CCan1939::Get8L(pPacket,3);

					//�^�[�Q�b�g������w�肳�ꂽ���M���p�P�b�g����ۑ�
					nMaxPacket = pPacket[4];

					//�p�P�b�g�ԍ���������
					nPacketNum = 1;

					//CTS��ԓ�����X�e�[�W�ɑJ��
					nStage = 4;
					}
				//CM_EOMA?
				else if(nType == CCan1939::DEF_EOMA)
					{
					nStage = 6;
					}
				//CM_ABORT?
				else if(nType == CCan1939::DEF_ABORT)
					{
					//���~����
					nExitCode = -11;
					nStage = 99;
					}
				//��L�ȊO�H
				else
					{
					//��L�ȊO�͌��Ȃ��������ɂ��čđ���҂�
					Sleep(0);
					}
				}
			else
				{
				//��M�������Ɏ��s�AABORT���M
				nExitCode = -14;
				nAbort = 3;	//Timeout
				nStage = 10;	
				}
			//
			CCan1939::FreeBuffer(pPacket);
			}
		//CTS�ԓ��X�e�[�W
		//	PC����M�m�[�h�EABH3�����M�m�[�h�ŁA��M�v��(CM_RTS)���̓f�[�^(CM_DT)�̕ԓ�(CM_CTS)���K�v�Ȃ̂ł���𑗐M
		else if(nStage == 4)
			{
			//���M���p�P�b�g�̓^�[�Q�b�g�����w�肵���l�����̂܂ܗ��p

			//CM_CTS�p�P�b�g������đ��M
			uint8_t nRemainPacket = nMaxPacket - nPacketNum + 1;				//�c��̃p�P�b�g��
			uint8_t* pPacket = CCan1939::CreateCMCTS(nRemainPacket,nPacketNum);
			nResult = CanSend8(nSendID,pPacket,8);
			CCan1939::FreeBuffer(pPacket);
			//CM_CTS���M����H
			if(nResult == 0)
				nStage = 5;
			else
				{
				//���M���s
				nExitCode = -13;
				nAbort = 2;	//resource�s��
				nStage = 10;
				}
			}

		//DT��M�X�e�[�W
		//	PC����M�m�[�h�EABH3�����M�m�[�h�ŁA�v��(CM_CTS)���̓f�[�^(CM_DT)�̕ԓ�(CM_RTS)���K�v�Ȃ̂ł���𑗐M
		else if(nStage == 5)
			{
			//debug
			if(nMaxPacket < 1)
				{
				nMaxPacket = 1;
				}

			//��M�̈�\�z
			bool bRetry = false;
			uint8_t* pPacket = CCan1939::CreateBuffer();

			//���M�����f�[�^���i�\�萔�j�̐�������M�������s��
			for(uint8_t nLoop = 0;nLoop < nMaxPacket;nLoop++)
				{
				//��M
				nResult = CanRecv8(&nID,pPacket);
				//��M�͐���H
				if(nResult == 0)
					{
					//�V�[�P���X�ԍ����قȂ�H
					if(pPacket[0] != nPacketNum)
						{
						//�v�������̂ƈقȂ镨���͂����̂ŁA�ēx�v������
						bRetry = true;
						break;
						}
					
					//�f�[�^�i�[�i7�o�C�g�j
					CCan1939::SetData(pRecvData,nRecvDataSize,CCan1939::packetnum2datapt(nPacketNum),&pPacket[1],7);

					//�ŏI�p�P�b�g�ԍ��H
					if(nPacketNum == nTotalPacket)
						break;	//����

					//�v�������p�P�b�g�ԍ��œ͂����̂ŁA���̃p�P�b�g�ԍ��ɍX�V
					++nPacketNum;
					}
				else
					{
					//��M�����Ɏ��s
					nExitCode = -14;
					nAbort = 3;	//Timeout
					nStage = 10;
					break;
					}
				}
			//��M�̈�J��
			CCan1939::FreeBuffer(pPacket);

			//���g���C���K�v�H
			if(bRetry)
				{
				//�p�P�b�g�ԍ�(nPacketNum)���������Ȃ��ӏ�����ēx�f�[�^(CM_DT)��v������
				nStage = 4;
				}

			//����H
			else if(nResult == 0)
				{
				if(nPacketNum == nTotalPacket)
					nStage = 6;	//EOMA���M�X�e�[�W�ɑJ��
				else
					nStage = 4;	//CTS�ԓ��ɖ߂�
				}

			else
				{
				//��M���s�A��M���[�`�����ňڍs�悪�ݒ肳��Ă���ׁA�������Ȃ�
				Sleep(0);
				}
			}

		//EOMA�ԓ��X�e�[�W
		else if(nStage == 6)
			{
			//�p�P�b�g������đ��M
			uint8_t* pPacket = CCan1939::CreateCMEOMA(nTotalPacket * 8,nTotalPacket);
			nResult = CanSend8(nSendID,pPacket,8);
			CCan1939::FreeBuffer(pPacket);
			//���M����H
			if(nResult == 0)
				{
				//�����X�e�[�W�Ɉڍs
				nStage = 7;
				}
			else
				{
				//���M���s
				nExitCode = -13;
				nAbort = 2;
				nStage = 10;
				}
			}

		//�����X�e�[�W
		else if(nStage == 7)
			{
			//���튮��
			break;
			}

		//�z�X�g�����璆�~�v���X�e�[�W
		//	CM_ABORT�𑗂�AnAbort�ɗ��R�����Ă���(2...���\�[�X�s��  3..�^�C���A�E�g)
		else if(nStage == 10)
			{
			//ABORT�p�P�b�g�����
			uint8_t* pPacket = CCan1939::CreateCMABORT(nAbort);
			nResult = CanSend8(nSendID,pPacket,8);
			CCan1939::FreeBuffer(pPacket);
			//
			nStage = 99;
			}

		//��L�ȊO�i�G���[�X�e�[�W�j
		else
			{
			//�ُ�I��
			break;
			}
		}

	//�I��
	return(nExitCode);
	}

//�߂�l�p�\���̂ɍŌ�ɑ��M�����f�[�^����������
void CAbh3::SetLastData(pCANABH3_RESULT pDst)
	{
	//�ۑ����Ă��鑗�M�v�f����������
	pDst->DP0S.nOrderAY		= m_var.lastdata.send.nOrderAY;
	pDst->DP0S.nOrderBX		= m_var.lastdata.send.nOrderBX;
	pDst->DP0S.nInputBit	= m_var.lastdata.send.nInputBit;
	}

