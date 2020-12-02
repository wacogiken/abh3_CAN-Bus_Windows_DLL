#pragma once

#include <Windows.h>
#include <stdint.h>
#include "CANa31dll.h"

//============================================================
//エクスポート？
//============================================================

 #ifndef _NO_CANA31API_EXPORT
  #define CANA31API __declspec(dllexport)
#else
  #define CANA31API __declspec(dllimport)
#endif

//============================================================
//以下リンケージはC言語形式とする
//============================================================

#ifdef __cplusplus
 extern "C" {
#endif //__cplusplus

//============================================================
//ユーザー用関数
//============================================================

//使用開始時に呼び出す必要が有ります
CANA31API void InitInstance(int32_t nIFnum);

//使用終了時に呼び出す必要が有ります
CANA31API void ExitInstance(void);

//使用可能なCANインターフェース数を取得
CANA31API int32_t GetInterfaceCount(void);

//指定インターフェースを開く
CANA31API int32_t OpenInterface(int32_t nDeviceNum);

//開いたインターフェースを閉じる
CANA31API void CloseInterface(void);

//現在開いているインターフェース番号を取得
CANA31API int32_t GetCurrentInterface(void);

//現在インターフェースを開いているか？
CANA31API int32_t IsOpenInterface(void);

//インターフェースを開くタイムアウト時間を設定
CANA31API void SetOpenTimeout(uint32_t nTimeoutMS);

//送信タイムアウト時間を設定
CANA31API void SetSendTimeout(uint32_t nTimeoutMS);

//受信タイムアウト時間を設定
CANA31API void SetRecvTimeout(uint32_t nTimeoutMS);

//通信対象ABH3のアドレスを設定
CANA31API void SetTargetID(uint8_t nAdrs);

//通信対象ABH3のアドレスを取得
CANA31API uint8_t GetTargetID(void);

//通信ホストのアドレスを設定
CANA31API void SetHostID(uint8_t nAdrs);

//通信ホストのアドレスを取得
CANA31API uint8_t GetHostID(void);

//通信速度を指定
CANA31API void SetBaudrate(uint32_t nBaudrateKbps);

//設定した通信速度を取得
CANA31API uint32_t GetBaudrate(void);

//PCが起動してからの時間を[ms]単位で取得
CANA31API uint32_t GetTm(void);

//指令の初期化
CANA31API int32_t abh3_can_init(void);

//指令の送信（軸別）
CANA31API int32_t abh3_can_cmdAY(int16_t cmd,pCANABH3_RESULT pPtr);
CANA31API int32_t abh3_can_cmdBX(int16_t cmd,pCANABH3_RESULT pPtr);

//指令の送信（同時）
CANA31API int32_t abh3_can_cmd(int16_t cmdAY,int16_t cmdBX,pCANABH3_RESULT pPtr);

//入力の送信（一括）
CANA31API int32_t abh3_can_inSet(int32_t data,int32_t mask,pCANABH3_RESULT pPtr);

//入力の送信（ビット）
CANA31API int32_t abh3_can_inBitSet(int8_t num,int8_t data,pCANABH3_RESULT pPtr);

//積算値のリクエスト
CANA31API int32_t abh3_can_reqPulse(pCANABH3_RESULT pPtr);

//ブロードキャストパケットのリクエスト
CANA31API int32_t abh3_can_reqBRD(uint8_t num,pCANABH3_RESULT pPtr);

//マルチパケットによるTelABH3パケットの送受信
CANA31API int32_t abh3_can_trans(char* sbuf,char* rbuf,size_t& rbuflen);

//速度から内部値へ変換
CANA31API int16_t	cnvVel2CAN(float vel);
CANA31API float	cnvCAN2Vel(int16_t vel);

//内部値からトルクへ変換
CANA31API float	cnvCAN2Trq(int16_t trq);

//内部値から負荷率に変換
CANA31API float	cnvCAN2Load(int16_t load);

//内部値からアナログ入力に変換
CANA31API float	cnvCAN2Analog(int16_t analog);

//内部値から電源電圧に変換
CANA31API float	cnvCAN2Volt(int16_t volt);

//============================================================
//リンケージ解除
//============================================================
#ifdef __cplusplus
 }
#endif //__cplusplus
