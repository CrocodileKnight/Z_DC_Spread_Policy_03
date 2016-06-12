/*
 * BreakPolicy.h
 *
 *  Created on: Dec 6, 2014
 *      Author: zhou
 */

#ifndef BREAKPOLICY_H_
#define BREAKPOLICY_H_
#include <deque>
#include <list>
#include <iostream>
#include "ThostFtdcUserApiDataType.h"
#include "qtapi.h"
#include "pBase.h"
#include "PolicyBase/MyDMA.h"
#include "PolicyBase_ext/MyDCM.h"
using namespace std;

static int MaxBit = 30;

class Z_DC_Spread_Policy : public IPolicy{
public:
	Z_DC_Spread_Policy();
	virtual ~Z_DC_Spread_Policy();
	virtual void OnStarted();
	virtual void OnNewFutureMD(MarketDataField* pMD);
	virtual void OnTimer(OrderField * pField,long nAction);
	/**
	 * 有报单时触发
	 */
	virtual void OnFOrderChanged(OrderField* pOrder,int nLastTradedVolume,int nFilledVolume,char cOrderStatus);

	virtual void OnFOrderFilled(OrderField* pOrder,TradeField* pTrade);

	//called by the framework when a bar in a series is opened;
	virtual void OnFBarClosed(IKBar* pBar,IKBarSeries* pSeries);
	virtual void OnFBarOpened(IKBar* pBar,IKBarSeries* pSeries);
	/**
	 * 设置交易账号
	 */
	virtual void OnSetFutureTrader(const char *Alias,IFutureTrader* pTrader);
	virtual void OnSetParam(const char* Name,const char * Value);
	virtual void Release();
	virtual void ClosePos();
	virtual void OnStoped(bool willDelete);


//Base
	void OrderChange(Member *member); 		//处理订单变化
	void OrderFilled(Member *member, TradeField* pTrade);		//处理订单成交
	void CatchOrder(Member *member);  		//处理追单
	void UpdateMD(MarketDataField* pMD);
	void UpdatePosProfit(char *buf);
	void UpdateWithDraw(char *buf);
	void UpdateCloseProfit(char *buf);
	void SavePos(const Member* member,bool isLongPos);
	void CalCloseProfit(Member *member,TradeField* pTrade);
	void CalProfit();
	double GetProfit(const Member *member);
	double GetWinPoint(const Member *member);
	double GetBaseWinRate(const Member *member);
	void OpenLong(Member *member);
	void OpenShort(Member *member);
	void CloseAllPos();
	void CloseLong(Member *member);
	void CloseShort(Member *member);
	double  GetPriceForBuy(const Member *member);
	double	GetPriceForSell(const Member *member);
	bool IsTradeLimit(Member* member,bool IsSurged);
	bool IsTradeLimit(bool IsAdd);

	void AddLong(Member *member,bool isWin);
	void AddLong(Member *member,int vol);
	void AddShort(Member *member,bool isWin);
	void AddShort(Member *member,int vol);
	void SubtractLong(Member *member,bool isWin);
	void SubtractLong(Member *member,int vol);
	void SubtractShort(Member *member,bool isWin);
	void SubtractShort(Member *member,int vol);

private:
	void Init();
	bool GetBarCloseFlag(IKBarSeries* pSeries,MarketDataField* pMD,int time);
	void ShowInfo(Member* member,const char buf[],double value);		//信息显示
	bool IsCallAuction(MarketDataField* pMD);//集合竞价
	void GetStartTime(char *ins);

	int SetBit(int &Value,int index);
	int ClearBit(int &Value,int index);
	int GetBit(const int Value,int index);
	int GetCurrentBit(double rate);
	void ManageRateSingal();

	void SetBaseFlag(Member* member);
	//void GetSma();
	//double LLV(int n);
	//double HHV(int n);
	//double SMA(const char *type,int n,int m);
private:
	//***************************************BASE
	IFutureTrader* m_pTrader;
	bool m_bStopOnClose;//强制平仓
	double m_fMaxLosePointInBars;
	int m_nInLoseCount;
	int m_nCurrentLoseCount;
	int m_nCurrentOpenTimes;
	int m_nInOpenTimes;
	double m_fCloseProfit;//平仓盈亏
	char m_cStartTime[10];
	string m_PrefixIns;//前缀
	//int m_nLoseCount;
	//MarketDataField* m_pLastMD;
	BarType m_nBarType;      //K线类型
	int m_nBarLength;        //K线长度
	BarType m_nBarType_1;      //K线类型
	int m_nBarLength_1;        //K线长度
	//止赢
	double m_fStopWinBase;
	double m_fStopWinFactor;
	double m_fStopLoseBase;
	int m_nSlipStep;          //开价滑点
	int updateTicks;
	//*****************************************
	//deque<IKBar> m_bRSIData;
	int m_nHistoryCount;

	bool m_bDirection;
	//double m_fCurrentPriceDisparity;
	//double m_fOpenPriceDisparity;
	//IKBarSeries * m_pSeries_ext;


	//**************************Create Member
	Member m_MyTradeA;
	Member m_MyTradeB;
	double m_fMaxProfit;
	double m_fCurrentProfit;
/*
	double m_fBasePriceLine;//基准线
	double m_fOffsetToClose;//平仓偏移价位
	double m_fOffsetToOpen;//开仓偏移价位
	double m_fOffSetToAdd[3];//每次加仓的价格偏移比例
	double m_fPosToAdd[3];//每次加仓占基本仓位的比例
	;
	double m_fJudgePriceValue;
	double m_fBaseValue;
	*/
	int m_nCurrentStage;//启动开仓控制
	double m_fAddPosInWin;
	double m_fAddPosInLose;
	double m_fIntervalInWin;
	double m_fIntervalInLose;
	//positive正
	//negative负
	int m_nPositiveAddEnableFlag;
	int m_nNegativeAddEnableFlag;//标记唤醒的加仓状态为
	int m_nPositiveAddedFlag;	 //标记唤醒的加仓为是否已经执行加仓
	int m_nNegativeAddedFlag;
	int m_nAddIntervalStep;
	//int m_nBaseIntervalStep;
	int m_nBaseAddTimes;			//基础仓位加仓次数

	int m_nAddTimesInDay;
	int m_nSubTimesInDay;
	int m_nTotalAddTimes;
	int m_nTotalSubTimes;
	//指标
	//list<CloseCombine> m_pList;
	//MyDCM *m_MyDCM;
	//MyDCM *m_MyDCM_1;
	//int m_nXor;
	//int m_nXor_1;

};

#endif /* BREAKPOLICY_H_ */
