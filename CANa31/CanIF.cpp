
//概要
//	CAN用デバイスクラス
//	（CANインターフェースの実装を行うクラスの継承元）
//
//使用方法
//	本クラスを継承したクラスを構築し、仮想関数全てをオーバライドし、処理を実装する
//	CAN制御クラス(CAbh3)等のコンストラクタに、継承したクラスを渡す
//	（継承したクラスは、CAN制御クラスの破棄完了まで、存在している必要が有る）
//

#include "stdafx.h"
#include "CanIF.h"
#include <timeapi.h>	//Windows.hより後になる様にインクルードする

//TimeAPI(GetTM関数の為に必要)
#pragma comment(lib,"winmm.lib")

//利用可能なCANインターフェース数を取得する場合に呼び出されます
int32_t CCanIF::OnGetInterfaceCount()
	{
	//概要
	//	指定CANインターフェースの利用可能数を取得する場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	利用可能インターフェース数

	//todo:CANインターフェースの数を戻す処理を実装して下さい

	return(0);
	}

//指定CANインターフェースを開く場合に呼び出されます
int32_t CCanIF::OnOpenInterface(int32_t nDeviceNum)
	{
	//概要
	//	指定CANインターフェースを開く場合に呼び出されます
	//パラメータ
	//	nDeviceNum		0開始のインターフェース番号
	//戻り値
	//	0				正常終了
	//	0以外			異常終了
	//注意点
	//	タイムアウト時間は、GetTimeout,GetTimeout_Open関数で取得出来ます

	//todo:CANインターフェースを開く処理を実装して下さい

	return(-1);
	}

//開いたCANインターフェースを閉じる場合に呼び出されます
void CCanIF::OnCloseInterface()
	{
	//概要
	//	開いたCANインターフェースを閉じる場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	無し

	//todo:CANインターフェースを開いている場合に閉じる処理を実装して下さい

	}

//CANインターフェースから受信する場合に呼び出されます
int32_t CCanIF::OnCanRecv(uint32_t* pCanID,uint8_t* pData8)
	{
	//概要
	//	CANインターフェースから1つ分のパケットを受信します
	//パラメータ
	//	pCanID			受信パケットのIDを格納する領域へのポインタ
	//	pData8			受信パケットを格納する8[bytes]領域へのポインタ
	//	nTimeoutMS		受信許容時間[ms]
	//戻り値
	//	0				正常終了
	//	0以外			異常終了

	//todo:CANインターフェースから1回分の受信を行う処理を実装して下さい

	return(-1);
	}

//CANインターフェースに送信する場合に呼び出されます
int32_t CCanIF::OnCanSend(uint32_t nCanID,uint8_t* pData8,uint8_t nLength)
	{
	//概要
	//	CANインターフェースに1つ分のパケットを送信します
	//パラメータ
	//	nCanID			送信パケットのID
	//	pData8			送信パケットが格納されている8[bytes]領域へのポインタ
	//	nTimeoutMS		送信許容時間[ms]
	//戻り値
	//	0				正常終了
	//	0以外			異常終了

	//todo:CANインターフェースに送信する処理を実装して下さい

	return(-1);
	}

//CANインターフェースにエラーが有るか調べる場合に呼び出されます
int32_t CCanIF::OnCanGetError()
	{
	//概要
	//	CANインターフェースにエラーが有るか調べます
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常

	//todo:CANインターフェースにエラーが有るか調べる処理を実装して下さい

	return(-1);
	}

//CANインターフェースのエラーを解除する場合に呼び出されます
int32_t CCanIF::OnCanClearError()
	{
	//概要
	//	CANインターフェースのエラーを解除します
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常

	//todo:CANインターフェースのエラーを解除する処理を実装して下さい

	return(-1);
	}

//受信バッファをクリアする場合に呼び出されます
int32_t CCanIF::OnCanRecvClear()
	{
	//概要
	//	受信バッファをクリアする場合に呼び出されます
	//パラメータ
	//	無し
	//戻り値
	//	0				正常
	//	0以外			異常

	//todo:受信バッファをクリアする処理を実装して下さい

	return(-1);
	}

//================================================================================
//上位からの設定を受ける関数
//================================================================================

//CANインターフェースのタイムアウトを設定
void CCanIF::SetTimeout(uint32_t nOpenMS,uint32_t nSendMS,uint32_t nRecvMS)
	{
	//概要
	//	CANインターフェースのタイムアウトを一括設定
	//パラメータ
	//	nOpenMS		回線を開く時のタイムアウト[ms]
	//	nSendMS		送信時のタイムアウト[ms]
	//	nRecvMS		受信時のタイムアウト[ms]
	//戻り値
	//	無し

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

//現在のCANインターフェースタイムアウト設定を取得
void CCanIF::GetTimeout(uint32_t& nOpenMS,uint32_t& nSendMS,uint32_t& nRecvMS)
	{
	//概要
	//	CANインターフェースのタイムアウトを一括取得
	//パラメータ
	//	nOpenMS		回線を開く時のタイムアウト[ms]を格納する参照
	//	nSendMS		送信時のタイムアウト[ms]を格納する参照
	//	nRecvMS		受信時のタイムアウト[ms]を格納する参照
	//戻り値
	//	無し
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

//CANインターフェースのボーレートを設定
void CCanIF::SetBaudrate(uint32_t nBaudrate)
	{
	//概要
	//	CANインターフェースのボーレートを設定
	//パラメータ
	//	nBaudrate		デバイス依存のボーレートを指定する値
	//戻り値
	//	無し
	//注意点
	//	設定値の単位等は、デバイスに依存する
	//	
	m_var.nBaudrate = nBaudrate;
	}

//現在のCANインターフェースのボーレートを取得
uint32_t CCanIF::GetBaudrate(void)
	{
	return(m_var.nBaudrate);
	}

//無視する発信元IDを指定
void CCanIF::SetIgnoreID(uint8_t nID)
	{
	m_var.nIgnoreID = nID;
	}

//無視する発信元IDを取得
uint8_t CCanIF::GetIgnoreID(uint8_t nID)
	{
	return(m_var.nIgnoreID);
	}

//唯一の受信対象IDを設定
void CCanIF::SetRecvOnlyID(uint32_t nID)
	{
	//パラメータ
	//	nID		唯一の受信対象となるID（0指定で解除）
	m_var.nOnlyID = nID;
	}

//設定済み唯一の受信対象IDを取得
uint32_t CCanIF::GetRecvOnlyID()
	{
	return(m_var.nOnlyID);
	}

//================================================================================
//上位含めて利用可能な関数
//================================================================================

//現在時刻を[ms]単位で取得
uint32_t CCanIF::GetTm()
	{
	MMTIME x;
	timeGetSystemTime(&x,sizeof(MMTIME));
	return(uint32_t(x.u.ticks));
	}
