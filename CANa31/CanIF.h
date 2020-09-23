#pragma once

//
#include <Windows.h>
#include <stdint.h>

//
class CCanIF
{
public:
//================================================================================
//使用CANインターフェース毎に実装が必須の仮想関数
//================================================================================
public:

	//利用可能なCANインターフェース数を取得する場合に呼び出されます
	virtual int32_t OnGetInterfaceCount(void);

	//指定CANインターフェースを開く場合に呼び出されます
	virtual int32_t OnOpenInterface(int32_t nDeviceNum);

	//開いたCANインターフェースを閉じる場合に呼び出されます
	virtual void OnCloseInterface(void);

	//CANインターフェースから受信する場合に呼び出されます
	virtual int32_t OnCanRecv(uint32_t* pCanID,uint8_t* pData8);

	//CANインターフェースに送信する場合に呼び出されます
	virtual int32_t OnCanSend(uint32_t nCanID,uint8_t* pData8);

	//CANインターフェースにエラーが有るか調べる場合に呼び出されます
	virtual int32_t OnCanGetError(void);

	//CANインターフェースのエラーを解除する場合に呼び出されます
	virtual int32_t OnCanClearError(void);

	//受信バッファをクリアする場合に呼び出されます
	virtual int32_t OnCanRecvClear(void);

//================================================================================
//設定用関数
//================================================================================
public:

	//CANインターフェースのタイムアウトを設定
	void SetTimeout(uint32_t nOpenMS,uint32_t nSendMS,uint32_t nRecvMS);
	void SetTimeout_Open(uint32_t nTimeoutMS);
	void SetTimeout_Send(uint32_t nTimeoutMS);
	void SetTimeout_Recv(uint32_t nTimeoutMS);

	//現在のCANインターフェースタイムアウト設定を取得
	void GetTimeout(uint32_t& nOpenMS,uint32_t& nSendMS,uint32_t& nRecvMS);
	uint32_t GetTimeout_Open(void);
	uint32_t GetTimeout_Send(void);
	uint32_t GetTimeout_Recv(void);

	//CANインターフェースのボーレートを設定
	void SetBaudrate(uint32_t nBaudrate);

	//現在のCANインターフェースのボーレートを取得
	uint32_t GetBaudrate(void);

	//無視する発信元IDを指定
	void SetIgnoreID(uint8_t nID);

	//無視する発信元IDを取得
	uint8_t GetIgnoreID(uint8_t nID);

	//唯一の受信対象IDを設定
	void SetRecvOnlyID(uint32_t nID);

	//設定済み唯一の受信対象IDを取得
	uint32_t GetRecvOnlyID(void);

//================================================================================
//上位含めて利用可能な関数
//================================================================================
public:

	//現在時刻を[ms]単位で取得
	uint32_t GetTm(void);

//================================================================================
//継承先のみ利用可能な関数
//================================================================================
protected:

	//指定IDが無視する対象か調べます
	bool IsIgnoreID(uint32_t nID)
		{
		//注意点
		//	優先度順に評価する為、else禁止


		//指定ID自体が0か？（データパケットとしては有りえない）
		if(nID == 0)
			return(true);		//正常なパケットではないので無視する

		//唯一の受信指定が有るか？
		if(m_var.nOnlyID)
			{
			//一致している？
			if(m_var.nOnlyID == nID)
				return(false);	//一致しているので無視しない
			return(true);		//一致していないので無視する
			}

		//特定発信元を無視する設定か？
		if(m_var.nIgnoreID)
			{
			//指定IDの発信元が一致するか？
			if(m_var.nIgnoreID == uint8_t(nID))
				return(true);	//一致しているので無視する
			}
		
		//条件に合わないので無視しない
		return(false);
		}

//================================================================================
//内部用
//================================================================================
private:

	//内部変数
	typedef struct _CANIF_VAR
		{
		uint32_t	nBaudrate;
		uint8_t		nIgnoreID;		//0以外で同じ値の発信元(IDの下位8bit)を無視する
		uint32_t	nOnlyID;		//0以外でこのIDのみ受信する（nIgnoreIDより優先）
		struct _TMO
			{
			uint32_t	nOpen;
			uint32_t	nSend;
			uint32_t	nRecv;
			} timeout;
		} CANIF_VAR,*pCANIF_VAR;
	CANIF_VAR m_var;



public:
	CCanIF()
		{
		::memset(&m_var,0,sizeof(CANIF_VAR));
		}
	virtual ~CCanIF()
		{
		}
};

