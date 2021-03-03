#pragma once

//このファイルは、ユーザー側でのみ使用します
//（DLL構築時には不要）

#include <Windows.h>
#include <stdint.h>
#include "typedef.h"

//============================================================
//エクスポートする関数を簡単に扱う為の要素
//============================================================

//使用開始時に呼び出す必要が有ります
typedef void(*CA3_InitInstance)(int32_t);

//使用終了時に呼び出す必要が有ります
typedef void (*CA3_ExitInstance)(void);

//使用可能なCANインターフェース数を取得
typedef int32_t(*CA3_GetInterfaceCount)(void);

//指定インターフェースを開く
typedef int32_t(*CA3_OpenInterface)(int32_t);

//開いたインターフェースを閉じる
typedef void(*CA3_CloseInterface)(void);

//現在開いているインターフェース番号を取得
typedef int32_t(*CA3_GetCurrentInterface)(void);

//現在インターフェースを開いているか？
typedef int32_t(*CA3_IsOpenInterface)(void);

//インターフェースを開くタイムアウト時間を設定
typedef void(*CA3_SetOpenTimeout)(uint32_t);

//送信タイムアウト時間を設定
typedef void(*CA3_SetSendTimeout)(uint32_t);

//受信タイムアウト時間を設定
typedef void(*CA3_SetRecvTimeout)(uint32_t);

//通信対象ABH3のアドレスを設定
typedef void(*CA3_SetTargetID)(uint8_t);

//通信対象ABH3のアドレスを取得
typedef uint8_t(*CA3_GetTargetID)(void);

//通信ホストのアドレスを設定
typedef void(*CA3_SetHostID)(uint8_t);

//通信ホストのアドレスを取得
typedef uint8_t(*CA3_GetHostID)(void);

//通信速度を指定
typedef void(*CA3_SetBaudrate)(uint32_t);

//設定した通信速度を取得
typedef uint32_t(*CA3_GetBaudrate)(void);

//PCが起動してからの時間を[ms]単位で取得
typedef uint32_t(*CA3_GetTm)(void);

//指令の初期化
typedef int32_t(*CA3_abh3_can_init)(void);

//指令の送信（軸別）
typedef int32_t(*CA3_abh3_can_cmdAY)(int16_t,pCANABH3_RESULT);
typedef int32_t(*CA3_abh3_can_cmdBX)(int16_t,pCANABH3_RESULT);

//指令の送信（同時）
typedef int32_t(*CA3_abh3_can_cmd)(int16_t,int16_t,pCANABH3_RESULT);

//入力の送信（一括）
typedef int32_t(*CA3_abh3_can_inSet)(int32_t,int32_t,pCANABH3_RESULT);

//入力の送信（ビット）
typedef int32_t(*CA3_abh3_can_inBitSet)(int8_t,int8_t,pCANABH3_RESULT);

//積算値のリクエスト
typedef int32_t(*CA3_abh3_can_reqPulse)(pCANABH3_RESULT);

//ブロードキャストパケットのリクエスト
typedef int32_t(*CA3_abh3_can_reqBRD)(uint8_t,pCANABH3_RESULT);

//マルチパケットによるTelABH3パケットの送受信
typedef int32_t(*CA3_abh3_can_trans)(char*,char*,uint32_t*);

//速度
typedef int16_t(*CA3_cnvVel2CAN)(float vel);
typedef float(*CA3_cnvCAN2Vel)(int16_t vel);

//電流
typedef int16_t(*CA3_cnvCur2CAN)(float);
typedef float(*CA3_cnvCAN2Cur)(int16_t);

//負荷率
typedef float(*CA3_cnvCAN2Load)(int16_t);

//アナログ入力
typedef float(*CA3_cnvCAN2Analog)(int16_t);

//電源電圧
typedef float(*CA3_cnvCAN2Volt)(int16_t);


//デバッグ機能の有効無効設定
typedef int32_t(*CA3_SetDebugMode)(int32_t);

//デバッグ情報のクリア
typedef int32_t(*CA3_ClearDebugInfo)(uint8_t);

//デバッグ情報の取得
typedef int32_t(*CA3_GetDebugInfo)(int32_t,uint8_t*);



//============================================================
//DLLの全関数を扱う為の構造体
//============================================================

//DLLファンクション関数アドレス管理
typedef struct _CA3_FUNCLIST
	{
	//使用開始時に呼び出す必要が有ります
	CA3_InitInstance		InitInstance;
	//使用終了時に呼び出す必要が有ります
	CA3_ExitInstance		ExitInstance;
	//使用可能なCANインターフェース数を取得
	CA3_GetInterfaceCount	GetInterfaceCount;
	//指定インターフェースを開く
	CA3_OpenInterface		OpenInterface;
	//開いたインターフェースを閉じる
	CA3_CloseInterface		CloseInterface;
	//現在開いているインターフェース番号を取得
	CA3_GetCurrentInterface	GetCurrentInterface;
	//現在インターフェースを開いているか？
	CA3_IsOpenInterface		IsOpenInterface;
	//インターフェースを開くタイムアウト時間を設定
	CA3_SetOpenTimeout		SetOpenTimeout;
	//送信タイムアウト時間を設定
	CA3_SetSendTimeout		SetSendTimeout;
	//受信タイムアウト時間を設定
	CA3_SetRecvTimeout		SetRecvTimeout;
	//通信対象ABH3のアドレスを設定
	CA3_SetTargetID			SetTargetID;
	//通信対象ABH3のアドレスを取得
	CA3_GetTargetID			GetTargetID;
	//通信ホストのアドレスを設定
	CA3_SetHostID			SetHostID;
	//通信ホストのアドレスを取得
	CA3_GetHostID			GetHostID;
	//通信速度を指定
	CA3_SetBaudrate			SetBaudrate;
	//設定した通信速度を取得
	CA3_GetBaudrate			GetBaudrate;
	//PCが起動してからの時間を[ms]単位で取得
	CA3_GetTm				GetTm;
	//指令の初期化
	CA3_abh3_can_init		abh3_can_init;
	//指令の送信（軸別）
	CA3_abh3_can_cmdAY		abh3_can_cmdAY;
	CA3_abh3_can_cmdBX		abh3_can_cmdBX;
	//指令の送信（同時）
	CA3_abh3_can_cmd		abh3_can_cmd;
	//入力の送信（一括）
	CA3_abh3_can_inSet		abh3_can_inSet;
	//入力の送信（ビット）
	CA3_abh3_can_inBitSet	abh3_can_inBitSet;
	//積算値のリクエスト
	CA3_abh3_can_reqPulse	abh3_can_reqPulse;
	//ブロードキャストパケットのリクエスト
	CA3_abh3_can_reqBRD		abh3_can_reqBRD;
	//マルチパケットによるTelABH3パケットの送受信
	CA3_abh3_can_trans		abh3_can_trans;
	//速度
	CA3_cnvVel2CAN			cnvVel2CAN;
	CA3_cnvCAN2Vel			cnvCAN2Vel;
	//電流
	CA3_cnvCAN2Cur			cnvCAN2Cur;
	CA3_cnvCur2CAN			cnvCur2CAN;
	//負荷率
	CA3_cnvCAN2Load			cnvCAN2Load;
	//アナログ入力
	CA3_cnvCAN2Analog		cnvCAN2Analog;
	//電源電圧
	CA3_cnvCAN2Volt			cnvCAN2Volt;

	//

	//デバッグ機能の有効無効設定
	CA3_SetDebugMode		SetDebugMode;
	//デバッグ情報のクリア
	CA3_ClearDebugInfo		ClearDebugInfo;
	//デバッグ情報の取得
	CA3_GetDebugInfo		GetDebugInfo;

	} CA3_FUNCLIST,*pCA3_FUNCLIST;

//DLLの関数を一括で扱う為の関数定義
int32_t GetFunctions(HMODULE hDLL,pCA3_FUNCLIST pFuncList);
