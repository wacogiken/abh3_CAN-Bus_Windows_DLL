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
CANA31API int32_t abh3_can_reqPulse(pCANABH3_RESULT pPtr);

//�u���[�h�L���X�g�p�P�b�g�̃��N�G�X�g
CANA31API int32_t abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr);

//�}���`�p�P�b�g�ɂ��TelABH3�p�P�b�g�̑���M
CANA31API int32_t abh3_can_trans(char* sbuf,char* rbuf,size_t& rbuflen);

//���x��������l�֕ϊ�
CANA31API int16_t	cnvVel2CAN(float vel);
CANA31API float	cnvCAN2Vel(int16_t vel);

//�����l����g���N�֕ϊ�
CANA31API float	cnvCAN2Trq(int16_t trq);

//�����l���畉�ח��ɕϊ�
CANA31API float	cnvCAN2Load(int16_t load);

//�����l����A�i���O���͂ɕϊ�
CANA31API float	cnvCAN2Analog(int16_t analog);

//�����l����d���d���ɕϊ�
CANA31API float	cnvCAN2Volt(int16_t volt);

//============================================================
//�����P�[�W����
//============================================================
#ifdef __cplusplus
 }
#endif //__cplusplus
