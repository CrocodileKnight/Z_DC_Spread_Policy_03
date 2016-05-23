/*
 * pBase.h
 *
 *  Created on: Dec 26, 2014
 *      Author: zhou
 */

#ifndef PBASE_H_
#define PBASE_H_
#include "ThostFtdcUserApiDataType.h"
#include "qtapi.h"
#include "PolicyBase/MyKDXZ.h"
#include <iostream>
using namespace std;

#define LONG_ADD_POS_B			"long_add_pos_b"
#define LONG_POS_B 				"long_pos_b"
#define LONG_POS_PRICE_B 		"long_pos_price_b"
#define HIGH_SINCE_OPEN_LONG_B 	"high_since_open_long_b"
#define LOW_SINCE_OPEN_LONG_B 	"low_since_open_long_b"

#define SHORT_ADD_POS_B			"short_add_pos_b"
#define SHORT_POS_B 			"short_pos_b"
#define SHORT_POS_PRICE_B 		"short_pos_price_b"
#define HIGH_SINCE_OPEN_SHORT_B	"high_since_open_short_b"
#define LOW_SINCE_OPEN_SHORT_B 	"low_since_open_short_b"
//***************************************
#define LONG_ADD_POS_A			"long_add_pos_a"
#define LONG_POS_A 				"long_pos_a"
#define LONG_POS_PRICE_A 		"long_pos_price_a"
#define HIGH_SINCE_OPEN_LONG_A 	"high_since_open_long_a"
#define LOW_SINCE_OPEN_LONG_A 	"low_since_open_long_a"

#define SHORT_ADD_POS_A			"short_add_pos_a"
#define SHORT_POS_A 			"short_pos_a"
#define SHORT_POS_PRICE_A 		"short_pos_price_a"
#define HIGH_SINCE_OPEN_SHORT_A "high_since_open_short_a"
#define LOW_SINCE_OPEN_SHORT_A 	"low_since_open_short_a"

#define STOP_PRICE 				"stop_price"

#define POS_PROFIT				"_positionprofit"
#define CLOSE_PROFIT			"_closeprofit"
#define WITHDRAW				"_withdraw"
#define STATUSMSG				"_statusmsg"
#define LONGPOS					"long_pos"
#define SHORTPOS				"short_pos"





enum TradeState{
	Trade_Normal,
	Trade_Buying,
	Trade_Shorting,
	Trade_Selling,
	Trade_BuyingAdd,
	Trade_ShortingAdd,
	Trade_Coverring,
};

enum ClosePosState{
	Close_Normal,
	Close_TimeOver,
	Close_StopWin,
	Close_StopLose,
	Close_Standard,
	Close_SubtractPos,
	Close_Force,
};


struct Member{
	FutureInstrumentField 	InsField;
	MarketDataField* 		m_pLastMD;
	IKBarSeries*   			m_pSeries;
	IKBarSeries* 			m_pSeries_1;
	OrderField*   			m_pOrder;
	IKBar* 		    		m_pLastBar;
	IKBar* 		    		m_pNewBar;
	char m_InstrumentID[32]; //合约代码
	char m_CountID[32];
	int m_nLongPos;
	int m_nShortPos;
	int m_nMaxPos;
	int m_nMaxAddPos;
	int m_nLongAddPos;
	int m_nShortAddPos;
	//int m_nOpenStage;//记录当前开加仓处于那一阶段
	int m_nMaxAddPosInWin;
	int m_nMaxAddPosInLose;
	int m_nOpenDirection;//-1开空，1开多；//
	double m_fBasePrice;//记录开仓起始价格

	double m_fHighSinceOpenLong;
	double m_fLowSinceOpenLong;
	double m_fHighSinceOpenShort;
	double m_fLowSinceOpenShort;
	double m_fAvgLongPrice;
	double m_fAvgShortPrice;
	Member(){
		memset(this,0,sizeof(Member));
	}
	void Clear(){
		this->m_pOrder = NULL;
		this->m_pLastMD = NULL;
		this->m_pLastBar = NULL;
		this->m_pNewBar = NULL;
	}
	virtual ~Member(){
		//delete m_nKDXZ;
	}
};


struct CloseCombine{
	double close1;
	double close2;
	double value;
	CloseCombine(){
		memset(this,0,sizeof(CloseCombine));
	}
	virtual ~CloseCombine(){
	}
};
/*
class pBase {
public:
	pBase();
	virtual ~pBase();
	OrderField* CatchOrder(OrderField* pOrder,MarketDataField* pMD){return NULL;};
	void UpdateMD(MarketDataField* pMD){};
	virtual void UpdatePosProfit(char *buf){};
	virtual void UpdateWithDraw(char *buf){};
	virtual void UpdateCloseProfit(char *buf){};
	virtual void SavePos(bool isLongPos){};
	virtual void CalProfit(){};
	virtual void CalCloseProfit(bool isLong,TradeField* pTrade){};
	virtual void OpenLong(char *Ins){};
	virtual void OpenShort(char *Ins){};
	virtual void CloseLong(ClosePosState state){};
	virtual void CloseShort(ClosePosState state){};

protected:

	IFutureTrader* m_pTrader;
	bool m_bStopOnClose;//强制平仓
	double m_fMaxLosePointInBars;
	int m_nInLoseCount;
	int m_nCurrentLoseCount;
	int m_nCurrentOpenTimes;
	int m_nInOpenTimes;

	double m_fCloseProfit;//平仓盈亏




	//int m_nLoseCount;

	//MarketDataField* m_pLastMD;
	BarType m_nBarType;      //K线类型
	int m_nBarLength;        //K线长度
	//止赢
	double m_fStopWinBase;
	double m_fStopWinFactor;
	double m_fStopLoseBase;
	int m_nSlipStep;          //开价滑点
	int updateTicks;
};
*/
#endif /* PBASE_H_ */
