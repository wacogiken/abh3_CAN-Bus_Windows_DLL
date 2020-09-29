#pragma once

//�W���I�ȃC���N���[�h
#include <Windows.h>
#include <stdint.h>

//�p�����N���X
#include "CanIF.h"

//�{�N���X�Ŏg�p����f�o�C�X�p�̃C���N���[�h
#define INITGUID
#include "vcisdk.h"
#include "vcitype.h"

//�{�N���X��`
class CIxxatV2 : public CCanIF
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
	//
	typedef struct _IXXATV2_STATUS
		{
		bool	bCanStart;	//false..stopped or reseted  true..started
		union
			{
			DWORD nRaw;
			struct _errbit_info
				{
				DWORD	nStuff:1;
				DWORD	nForm:1;
				DWORD	nACK:1;
				DWORD	nBit:1;
				DWORD	nCRC:1;
				DWORD	nOther:1;
				DWORD	nDummy:26;
				} info;
			} err;
		} IXXATV2_STATUS,*pIXXATV2_STATUS;
	//
	IXXATV2_STATUS	m_status;
	IBalObject*		m_pBalObject;		// bus access object
	LONG			m_lSocketNo;		// socket number
	ICanControl*	m_pCanControl;		// control interface
	ICanChannel*	m_pCanChn;			// channel interface
	LONG			m_lMustQuit;		// quit flag for the receive thread
	HANDLE			m_hEventReader;		//
	PFIFOREADER		m_pReader;			//
	PFIFOWRITER		m_pWriter;			//
	HANDLE			m_hSema;			//��M�o�b�t�@�r������p�Z�}�t�H
	HANDLE			m_hReadThread;		//��M�X���b�h�n���h��
	PCANMSG			m_pCanMsg;
	uint32_t		m_nCanReadPt;
	uint32_t		m_nCanWritePt;

	//�񓯊���M�X���b�h
	static void ReceiveThread(void* Param);

	//��M�o�b�t�@�r������v��
	bool LockCanMsg(DWORD nTimeoutMS = INFINITE);

	//��M�o�b�t�@�r���������
	void UnlockCanMsg(void);

	//��M�f�[�^�̓o�^����
	uint32_t AddCanMsg(PCANMSG pMsg,int nCount = 1);

	//�o�^���ꂽCAN���b�Z�[�W��1�擾
	uint32_t GetCanMsg(PCANMSG pMsg);

//================================================================================
//�����p
//================================================================================
protected:
	//
	void debugprint(bool bSend,uint32_t nCanID,uint8_t* pData)
		{
	//	printf("%8d %08X ",GetTm(),nCanID);
	//	if(!bSend)
	//		printf("\t");
	//	for(int32_t nLoop = 0;nLoop < 8;nLoop++)
	//		printf("%02X ",pData[nLoop]);
	//	printf("\n");
		}

public:
	CIxxatV2();
	virtual ~CIxxatV2();
};
