#pragma once

//�W���I�ȃC���N���[�h
#include <Windows.h>
#include <stdint.h>

//�p�����N���X
#include "CanIF.h"

//�{�N���X�Ŏg�p����f�o�C�X�p�̃C���N���[�h
#include "simply.h"

//�{�N���X��`
class CIxxatSimple : public CCanIF
{
public:
//================================================================================
//�g�pCAN�C���^�[�t�F�[�X���Ɏ������K�{�̉��z�֐�
//================================================================================
public:

	//���p�\��CAN�C���^�[�t�F�[�X�����擾����ꍇ�ɌĂяo����܂�
	virtual int32_t OnGetInterfaceCount(void);

	//�w��CAN�C���^�[�t�F�[�X���J���ꍇ�ɌĂяo����܂�
	virtual int32_t OnOpenInterface(int32_t nDeviceNum);

	//�J����CAN�C���^�[�t�F�[�X�����ꍇ�ɌĂяo����܂�
	virtual void OnCloseInterface(void);

	//CAN�C���^�[�t�F�[�X�����M����ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanRecv(uint32_t* pCanID,uint8_t* pData8);

	//CAN�C���^�[�t�F�[�X�ɑ��M����ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength);

	//CAN�C���^�[�t�F�[�X�ɃG���[���L�邩���ׂ�ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanGetError(void);

	//CAN�C���^�[�t�F�[�X�̃G���[����������ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanClearError(void);

	//��M�o�b�t�@���N���A����ꍇ�ɌĂяo����܂�
	virtual int32_t OnCanRecvClear(void);
	
//================================================================================
//�����p�i�X���b�h���痘�p�ׂ�public�����j
//================================================================================
public:
	//�f�o�C�X�ŗL�̃X�e�[�^�X���
	typedef struct _IXXATSMPL_STATUS
		{
		bool	bOpen;		//false(������J���ĂȂ�) true(������J���Ă���j
		union
			{
			uint16_t nRaw;
			struct _errbit_info
				{
				uint16_t	nRunning:1;
				uint16_t	nReset:1;
				uint16_t	nBusoff:1;
				uint16_t	nErrorStatus:1;
				uint16_t	nTXoverrun:1;
				uint16_t	nRXoverrun:1;
				uint16_t	nPending:1;
				uint16_t	nDummy:9;
				} info;
			} err;
		} IXXATSMPL_STATUS,*pIXXATSMPL_STATUS;
	//
	IXXATSMPL_STATUS	m_status;
	HANDLE				m_hSema;			//��M�o�b�t�@�r������p�Z�}�t�H
	HANDLE				m_hReadThread;		//��M�X���b�h�n���h��
	UINT				m_nThreadID;		//��M�X���b�h�̃v���Z�XID
	bool				m_bQuitThread;		//true�ŃX���b�h��~�v��
	can_msg_t*			m_pCanMsg;			//��M�o�b�t�@
	uint32_t			m_nCanReadPt;		//��M�o�b�t�@�̓ǂݏo���ʒu
	uint32_t			m_nCanWritePt;

	//�񓯊���M�X���b�h
	static unsigned int __stdcall ReceiveThread(void* Param);

	//��M�o�b�t�@�r������v��
	bool LockCanMsg(DWORD nTimeoutMS = INFINITE);

	//��M�o�b�t�@�r���������
	void UnlockCanMsg(void);

	//��M�f�[�^�̓o�^����
	uint32_t AddCanMsg(can_msg_t* pMsg,int nCount = 1);

	//�o�^���ꂽCAN���b�Z�[�W��1�擾
	uint32_t GetCanMsg(can_msg_t* pMsg);

//================================================================================
//�����p
//================================================================================
protected:
	//
	void debugprint(bool bSend,uint32_t nCanID,uint8_t* pData)
		{
		//printf("%8d %08X ",GetTm(),nCanID);
		//if(!bSend)
		//	printf("\t");
		//for(int32_t nLoop = 0;nLoop < 8;nLoop++)
		//	printf("%02X ",pData[nLoop]);
		//printf("\n");
		}

public:
	CIxxatSimple();
	virtual ~CIxxatSimple();
};
