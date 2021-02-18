//���d�C���N���[�h�h�~
#ifndef _ABH3_H_INCLUDED_
#define _ABH3_H_INCLUDED_

//�v���b�g�t�H�[���ˑ�
#include <Windows.h>
#include <stdint.h>
#include <timeapi.h>
#include <stdio.h>

//CAN�֘A
#include "Can1939.h"
#include "CanIF.h"

//�^��`
#include "typedef.h"

//�N���X��`
class CAbh3
{
//================================================================================
//���[�U�[�����p�\�Ȍ^
//================================================================================
public:

//================================================================================
//�����Ŏg�p�����^
//================================================================================
protected:

//================================================================================
//���[�U�[�����p�\�Ȋ֐��i��{�v�f�j
//================================================================================
public:
	//�g�p�\��CAN�C���^�[�t�F�[�X�����擾
	int32_t GetInterfaceCount(void);

	//�w��C���^�[�t�F�[�X���J��
	int32_t OpenInterface(int32_t nDeviceNum);

	//�J�����C���^�[�t�F�[�X�����
	void CloseInterface(void);

	//���݊J���Ă���C���^�[�t�F�[�X�ԍ����擾
	int32_t GetCurrentInterface(void);

	//���݃C���^�[�t�F�[�X���J���Ă��邩�H
	int32_t IsOpenInterface(void);

	//�C���^�[�t�F�[�X���J���^�C���A�E�g���Ԃ�ݒ�
	void SetOpenTimeout(uint32_t nTimeoutMS);

	//���M�^�C���A�E�g���Ԃ�ݒ�
	void SetSendTimeout(uint32_t nTimeoutMS);

	//��M�^�C���A�E�g���Ԃ�ݒ�
	void SetRecvTimeout(uint32_t nTimeoutMS);

	//�ʐM�Ώ�ABH3�̃A�h���X��ݒ�
	void SetTargetID(uint8_t nAdrs);

	//�ʐM�Ώ�ABH3�̃A�h���X���擾
	uint8_t GetTargetID(void);

	//�ʐM�z�X�g�̃A�h���X��ݒ�
	void SetHostID(uint8_t nAdrs);

	//�ʐM�z�X�g�̃A�h���X���擾
	uint8_t GetHostID(void);

	//�ʐM���x���w��
	void SetBaudrate(uint32_t nBaudrateKbps);

	//�ݒ肵���ʐM���x���擾
	uint32_t GetBaudrate(void);

	//PC���N�����Ă���̎��Ԃ�[ms]�P�ʂŎ擾
	uint32_t GetTm(void)
		{
		uint32_t nResult = 0;
		if(!IsValidDevice())
			nResult = m_var.pDeviceClass->GetTm();
		return(nResult);
		}	

//================================================================================
//���[�U�[�����p�\�Ȋ֐��iABH3�v�f�j
//================================================================================
public:
	//�w�߂̏�����
	int32_t abh3_can_init(void);

	//�w�߂̑��M�i���ʁj
	int32_t abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr);
	int32_t abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr);

	//�w�߂̑��M�i�����j
	int32_t abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr);

	//���͂̑��M�i�ꊇ�j
	int32_t abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr);

	//���͂̑��M�i�r�b�g�j
	int32_t abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr);

	//�ώZ�l�̃��N�G�X�g
	int32_t abh3_can_reqPulse(pCANABH3_RESULT pPtr);

	//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
	int32_t abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr);

	//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
	int32_t abh3_can_trans(char* sbuf,char*& rbuf,size_t& rbuflen);

//================================================================================
//���[�U�[�����p�\�Ȋ֐��i�ϊ��p�j
//================================================================================
public:
	//���x
	int16_t	cnvVel2CAN(float vel)
		{
		int16_t nResult = int16_t(vel / 0.2f);
		return(nResult);
		}
	float	cnvCAN2Vel(int16_t vel)
		{
		float nResult = float(vel) * 0.2f;
		return(nResult);
		}

	//�g���N
	float	cnvCAN2Trq(int16_t trq)
		{
		float nResult = float(trq) * 0.01f;
		return(nResult);
		}

	//���ח�
	float	cnvCAN2Load(int16_t load)
		{
		float nResult = float(load) * 1.0f;
		return(nResult);
		}

	//�A�i���O����
	float	cnvCAN2Analog(int16_t analog)
		{
		float nResult = float(analog) * 0.01f;
		return(nResult);
		}

	//�d���d��
	float	cnvCAN2Volt(int16_t volt)
		{
		float nResult = float(volt) * 0.1f;
		return(nResult);
		}

//================================================================================
//�����p
//================================================================================
protected:

	//�f�o�C�X�N���X���o�^����Ă��邩�H
	bool IsValidDevice(void)
		{
		//�o�^����Ă����true���߂�
		return((bool)(m_var.pDeviceClass != 0));
		}

	//�V���O���p�P�b�g��ID���\�z
	uint32_t CreateID(uint32_t nBaseID)
		{
		//����16bit�ɑ��M��Ƒ��M����ID������
		nBaseID &= 0xffff0000;
		nBaseID |= (m_var.config.nTargetAdrs << 8);
		nBaseID |= m_var.config.nHostAdrs;
		return(nBaseID);
		}

	//�u���[�h�L���X�g��ID���\�z
	uint32_t CreateBrdID(uint8_t nTarget)
		{
		//����8bit�Ƀ^�[�Q�b�g�̔ԍ�������
		uint32_t nResult = 0x00ff00 | nTarget;
		return(nResult);
		}

	//�r������v��
	int32_t Lock(void)
		{
		//
		if(!IsValidDevice())
			return(-1);	//�f�o�C�X�N���X���o�^����Ă��Ȃ�

		//���M�^�C���A�E�g���Ԃ��擾
		uint32_t nTimeoutMS = m_var.pDeviceClass->GetTimeout_Send();

		//���b�N���鎞�͑��M���e���Ԃ𗘗p����
		if(::WaitForSingleObject(m_var.hTerm,nTimeoutMS) != WAIT_OBJECT_0)
			return(-1);	//������L���s
		//������L����
		return(0);
		}

	//�r������J��
	void Unlock(void)
		{
		//���ӓ_
		//	�m�F���Ȃ��ŊJ������ׁA2�d�J���̗ނɒ���
		::ReleaseSemaphore(m_var.hTerm,1,NULL);
		}

	//��M�o�b�t�@�N���A
	void ClearRecvBuffer(void)
		{
		if(IsValidDevice())
			m_var.pDeviceClass->OnCanRecvClear();
		}

	//CAN���M
	int32_t CanSend8(uint32_t nSendID,uint8_t* pSendData,uint8_t nLength);

	//CAN��M
	int32_t CanRecv8(uint32_t* pRecvID,uint8_t* pRecvData); 

	//�V���O���p�P�b�g����M
	int32_t CanTermSingle(uint32_t nSendID,uint8_t* pSendData,pCANABH3_RESULT pResult,uint8_t nLength);
	int32_t CanTermSingle(uint32_t nSendID,uint32_t nOnlyID,uint8_t* pSendData,pCANABH3_RESULT pResult,uint8_t nLength);

	//�}���`�p�P�b�g���M
	int32_t CanTermSendMulti(uint8_t* pSendData,uint32_t nSendDataSize,uint8_t*& pRecvData,uint32_t& nRecvDataSize);

	//�߂�l�p�\���̂ɍŌ�ɑ��M�����f�[�^����������
	void SetLastData(pCANABH3_RESULT pDst);


	//�����p�ϐ�
	typedef struct _CANABH3_VAR
		{
		CCanIF*				pDeviceClass;	//�o�͐�̃N���X
		int32_t				nOpenDevice;	//0..�J���Ă��Ȃ�  1�ȏ�..�J�����C���^�[�t�F�[�X�ԍ�+1������
		HANDLE				hTerm;			//�ʐM�r���p�̃Z�}�t�H
		//
		struct _CANABH3_CONFIG
			{
			uint8_t			nTargetAdrs;	//ABH3���A�h���X
			uint8_t			nHostAdrs;		//�z�X�g���iPC�j�A�h���X
			} config;
		//
		struct _CANABH3_LASTDATA
			{
			//�ߋ��ɔc�������f�[�^�i���M�j
			struct _CANABH3_LASTDATA_SEND
				{
				uint32_t	nCanID;
				int16_t		nOrderAY;
				int16_t		nOrderBX;
				uint32_t	nInputBit;
				} send;
			//�ߋ��ɔc�������f�[�^�i��M�j
			struct _CANABH3_LASTDATA_RECV
				{
				//uint32_t	nCanID;
				int16_t		nBackSpeedA;
				int16_t		nBackSpeedB;
				int16_t		nBackSpeedY;
				int16_t		nBackSpeedX;
				int32_t		nInPulseA;
				int32_t		nInPulseB;
				uint32_t	nErrorBit;
				uint32_t	nWarnBit;
				uint32_t	nCtrlBit;
				uint32_t	nIOflag;
				int16_t		nOrderSpeedAY;
				int16_t		nOrderSpeedBX;
				int16_t		nBackSpeedAY;
				int16_t		nBackSpeedBX;
				int16_t		nOrderCurrentAY;
				int16_t		nOrderCurrentBX;
				int16_t		nLoadA;
				int16_t		nLoadB;
				int16_t		nAnalog0;
				int16_t		nAnalog1;
				int16_t		nPowerMain;
				int16_t		nPowerCtrl;
				float		nMonitor0;
				float		nMonitor1;
				} recv;

			} lastdata;

		//�f�o�b�O�p
		struct _CANABH3_DEBUG
			{
			bool			bEnable;	//�f�o�b�O�@�\�̗L�������ݒ�
			int				nInfoPt;	//���̏������ݏꏊ
			uint8_t*		pInfoData;	//9�o�C�g�P�ʂŎg��(�擪��0�ő��M�A1�Ŏ�M)
			} debug;

		} CANABH3_VAR,*pCANABH3_VAR;
	CANABH3_VAR m_var;

protected:

	//�f�o�b�O���̒ǉ�
	void AddDebugInfo(bool bSend,uint8_t* pData8)
		{
		//�f�o�b�O�@�\���L�����H
		if(m_var.debug.bEnable)
			{
//
int x = 0;
for(int nPt = 0;nPt < 8;nPt++)
	x += pData8[nPt];
if(x == 0)
Sleep(0);








			//���񗘗p�H
			if(m_var.debug.pInfoData == NULL)
				{
				m_var.debug.pInfoData = new uint8_t[512 * 9]();
				m_var.debug.nInfoPt = 0;
				}

			//�������߂邩�H
			if(m_var.debug.nInfoPt < 512)
				{
				//�z��ԍ�
				int nPt = 9 * m_var.debug.nInfoPt;
				//
				if(bSend)
					m_var.debug.pInfoData[nPt + 0] = 0;
				else
					m_var.debug.pInfoData[nPt + 0] = 1;
				memcpy(&m_var.debug.pInfoData[nPt + 1],pData8,8);
				//
				++m_var.debug.nInfoPt;
				}
			}
		}

public:
	//�f�o�b�O�@�\�̗L������
	void SetDebugMode(bool bEnable = false)
		{
		if(!bEnable)
			ClearDebugInfo();
		m_var.debug.bEnable = bEnable;
		}

	//�f�o�b�O���̏�����
	void ClearDebugInfo(void)
		{
		m_var.debug.nInfoPt = 0;
		if(m_var.debug.pInfoData)
			{
			delete m_var.debug.pInfoData;
			m_var.debug.pInfoData = NULL;
			}
		}

	//�f�o�b�O���̎擾
	uint8_t* GetDebugInfo(int nPt)
		{
		if(nPt < m_var.debug.nInfoPt)
			return(&m_var.debug.pInfoData[nPt * 9]);
		return(NULL);
		}

public:
	//�R���X�g���N�^
	CAbh3(CCanIF* pDeviceClass = NULL);
	//�f�X�g���N�^
	virtual ~CAbh3();
};

#endif
