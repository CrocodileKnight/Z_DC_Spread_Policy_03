/*
 * BreakPolicy.cpp
 *
 *  Created on: Dec 6, 2014
 *      Author: zhou
 */

#include "Z_DC_Spread_Policy.h"
#include <cmath>

Z_DC_Spread_Policy::Z_DC_Spread_Policy() {
	// TODO Auto-generated constructor stub
	//m_nMADays = 20;
	//strcpy(m_InstrumentID,"ag1506");
	cout<<"new Z_DC_Spread_Policy 02!"<<endl;
	m_nHistoryCount = 0;
	m_fMaxLosePointInBars = 100;
	//m_fOpenPrice = 0;
	updateTicks = 0;
	m_nSlipStep = 0;
	m_fCloseProfit = 0;
	m_nCurrentLoseCount = 0;
	m_nInLoseCount = 0;
	m_bDirection = true;
	m_nPositiveAddEnableFlag = 0;
	m_nNegativeAddEnableFlag = 0;
	m_nPositiveAddedFlag = 0;
	m_nNegativeAddedFlag = 0;
	m_nAddIntervalStep = 5;
	m_nAddTimesInDay = 0;
	m_nSubTimesInDay = 0;
	m_nTotalAddTimes = 0;
	m_nTotalSubTimes = 0;
	m_nBaseAddTimes = 0;


}

Z_DC_Spread_Policy::~Z_DC_Spread_Policy() {
	// TODO Auto-generated destructor stub
	//


}

void Z_DC_Spread_Policy::OnStarted()
{
	cout<<"OnStarted................."<<endl;
	Init();
	m_nCurrentOpenTimes = 0;
}

void Z_DC_Spread_Policy::OnTimer(OrderField* pField, long int nAction){
}

void Z_DC_Spread_Policy::OnFBarClosed(IKBar* pBar,IKBarSeries* pSeries)
{
	//cout<<"Z_DC_Spread_Policy Bar Closed"<<endl;
	m_MyTradeA.m_pLastBar = pBar;
}

void Z_DC_Spread_Policy::OnFBarOpened(IKBar* pBar,IKBarSeries* pSeries)
{
	//cout<<"bar start"<<endl;
}

double GetSecond(char* time){
	char hh[3];
	char mm[3];
	char ss[3];
	hh[0] = time[0];
	hh[1] = time[1];
	mm[0] = time[3];
	mm[1] = time[4];
	ss[0] = time[6];
	ss[1] = time[7];
	hh[2] = '\0';
	mm[2] = '\0';
	ss[2] = '\0';
	return atoi(hh)*60*60 + atoi(mm)*60 + atoi(ss);
}

bool GetTimeInterval(char* time1,char* time2){
	int _t1 = GetSecond(time1);
	int _t2 = GetSecond(time2);
	if(abs(_t1-_t2)<5){
		return true;
	}else
		return false;
}

bool Z_DC_Spread_Policy::GetBarCloseFlag(IKBarSeries* pSeries,MarketDataField* pMD,int time){
	IKBar* _lastbar = pSeries->GetBar(pSeries->m_nCount - 1);
	int _t1 = GetSecond(_lastbar->m_sEndTime);
	int _t2 = GetSecond(pMD->UpdateTime);
	if(_t1 - _t2 <= time){
		return true;
	}
	return false;
}
bool Z_DC_Spread_Policy::IsCallAuction(MarketDataField* pMD){//集合竞价时间不交易
	if(strcmp(pMD->UpdateTime,m_cStartTime) < 0 && strcmp(pMD->UpdateTime,"08:00:00") > 0){
		cout<<"Z_DC_Spread_Policy 02 IsCallAuction:"<<pMD->TradingDay<<" "<<pMD->UpdateTime<<endl;
		return true;
	}
	return false;
}
void Z_DC_Spread_Policy::OnNewFutureMD(MarketDataField* pMD)
{
	if(IsCallAuction(pMD)){
		return;
	}
	UpdateMD(pMD);
	if(m_bStopOnClose && m_MyTradeA.m_nLongPos<1 && m_MyTradeA.m_nShortPos<1 && m_MyTradeB.m_nLongPos<1 && m_MyTradeB.m_nShortPos<1){
		m_nCurrentStage = 0;
		m_nPositiveAddedFlag = 0;
		m_nPositiveAddEnableFlag = 0;
		m_nNegativeAddedFlag = 0;
		m_nNegativeAddEnableFlag = 0;
		m_MyTradeA.m_fBasePrice = 0;
		m_MyTradeB.m_fBasePrice = 0;
		m_nTotalAddTimes = 0;
		m_nTotalSubTimes = 0;
		SetAttr("totalAddtimes",m_nTotalAddTimes);
		SetAttr("totalSubTimes",m_nTotalSubTimes);
		SetAttr("current_stage",m_nCurrentStage);
		SetAttr("Padded_flag",m_nPositiveAddedFlag);
		SetAttr("Nadded_flag",m_nNegativeAddedFlag);
		SetAttr("Paddenable_flag",m_nPositiveAddEnableFlag);
		SetAttr("Naddenable_flag",m_nNegativeAddEnableFlag);
		SetAttr("base_price_a",0);
		SetAttr("base_price_b",0);
		Stop();
		ShowInfo(&m_MyTradeA,"Close stop.",0);
		return;
	}
	//if(m_MyTradeA.m_pLastMD != NULL && m_MyTradeB.m_pLastMD !=NULL){
		//m_fCurrentPriceDisparity = abs( m_MyTradeA.m_pLastMD->LastPrice - m_MyTradeB.m_pLastMD->LastPrice);
	//}
	if(!m_bStopOnClose && m_MyTradeA.m_pLastMD != NULL && m_MyTradeB.m_pLastMD !=NULL){
	/*	if(strcmp(pMD->InstrumentID, m_MyTradeA.m_InstrumentID)==0){	//For Test
			cout<<m_MyTradeA.m_pLastMD->UpdateTime<<" "<<m_MyTradeB.m_pLastMD->UpdateTime<<endl;
		}*/
		//cout<<(strcmp(pMD->InstrumentID, m_MyTradeA.m_InstrumentID) == 0)<<" "<<GetTimeInterval(m_MyTradeA.m_pLastMD->UpdateTime,m_MyTradeB.m_pLastMD->UpdateTime)<<" "<<m_nCurrentStage<<" "<<m_MyTradeA.m_nLongPos<<" "<<m_MyTradeB.m_nShortPos<<endl;
		if(strcmp(pMD->InstrumentID, m_MyTradeA.m_InstrumentID)==0 && GetTimeInterval(m_MyTradeA.m_pLastMD->UpdateTime,m_MyTradeB.m_pLastMD->UpdateTime)){
			if(m_nCurrentStage == 0 && m_MyTradeA.m_nLongPos==0 && m_MyTradeB.m_nShortPos == 0 && !IsTradeLimit(true)){//m_nCurrentStage == 0 &&
				ShowInfo(&m_MyTradeA,"Set Up Base Pos!",0);
				OpenLong(&m_MyTradeA);
				OpenShort(&m_MyTradeB);
				m_MyTradeA.m_fBasePrice = m_MyTradeA.m_pLastMD->LastPrice;
				m_MyTradeB.m_fBasePrice = m_MyTradeB.m_pLastMD->LastPrice;
				m_nCurrentStage = 1;

				SetBaseFlag(&m_MyTradeA);
				SetAttr("base_price_a",m_MyTradeA.m_fBasePrice);
				SetAttr("base_price_b",m_MyTradeB.m_fBasePrice);
				SetAttr("current_stage",m_nCurrentStage);
			}

			//判断当前盈利状况
			if(!this->GetBarCloseFlag(m_MyTradeA.m_pSeries,pMD,5)){//收盘前五秒没有买卖信号判断
				ManageRateSingal();
			}
			//if(m_MyTradeA.m_pLastMD!=NULL && m_MyTradeB.m_pLastMD!=NULL){
			CalProfit();
			//}
		}
	}

}



void Z_DC_Spread_Policy::OnFOrderChanged(OrderField* pOrder,int nLastTradedVolume,int nFilledVolume, char cOrderStatus)
{
	//cout<<"Order Change: "<<pOrder->InstrumentID<<" "<<pOrder<<" "<<m_MyTradeA.m_pOrder<<" "<<m_MyTradeB.m_pOrder<<" "<<pOrder->ErrorID<<" "<<cOrderStatus<<endl;
	if(pOrder == m_MyTradeA.m_pOrder){
		OrderChange(&m_MyTradeA);
	}
	else if(pOrder == m_MyTradeB.m_pOrder){
		OrderChange(&m_MyTradeB);
	}
}
void Z_DC_Spread_Policy::OnFOrderFilled(OrderField* pOrder,TradeField* pTrade)
{
	//cout<<"Order Filled:"<<pOrder->InstrumentID<<" "<<pTrade->InstrumentID<<endl;
	if(strcmp(pOrder->InstrumentID, m_MyTradeA.m_InstrumentID) == 0){
		cout<<"Trade A Order Filled"<<endl;
		OrderFilled(&m_MyTradeA, pTrade);
	}
	else if(strcmp(pOrder->InstrumentID, m_MyTradeB.m_InstrumentID) == 0){
		cout<<"Trade B Order Filled"<<endl;
		OrderFilled(&m_MyTradeB, pTrade);
	}
}


void Z_DC_Spread_Policy::OnSetFutureTrader(const char* Alias,IFutureTrader* pTrader)
{
	if(m_pTrader == NULL){
		m_pTrader = pTrader;
	}
}

void Z_DC_Spread_Policy::OnSetParam(const char* Name,const char * Value)
{

	cout<<"OnSetParam Spread02: "<<Name<<endl;
	if(strcmp(Name,"_acct") == 0){
		strcpy(m_MyTradeA.m_CountID,Value);
		strcpy(m_MyTradeB.m_CountID,Value);
	}
	if(strcmp(Name,"sIns1")==0){
		strcpy(m_MyTradeA.m_InstrumentID,Value);
		cout<<"instrumentID : "<<m_MyTradeA.m_InstrumentID<<endl;
	}else if(strcmp(Name,"sIns2")==0){
		strcpy(m_MyTradeB.m_InstrumentID,Value);
	}else if(strcmp(Name,"volume_a")==0){
		m_MyTradeA.m_nMaxPos = atoi(Value);
		cout<<"maxpos : "<<m_MyTradeA.m_nMaxPos<<endl;
	}else if(strcmp(Name,"volume_b")==0){
		m_MyTradeB.m_nMaxPos = atoi(Value);
		cout<<"maxpos : "<<m_MyTradeB.m_nMaxPos<<endl;
	}else if(strcmp(Name,"stopwinpoint")==0){
		m_fStopWinBase = atoi(Value);
	}else if(strcmp(Name,"stoplosepoint")==0){
		m_fStopLoseBase = atoi(Value);
	}else if(strcmp(Name,"stopwinrate")==0){
		//m_fStopWinRate = atof(Value);
		m_fStopWinFactor = atof(Value);
	}else if(strcmp(Name,"maxpointloseinbars") == 0){
		m_fMaxLosePointInBars = atof(Value);
	}else if(strcmp(Name,"slipstep") == 0){
		m_nSlipStep = atoi(Value);
	}else if(strcmp(Name,"losecount") == 0){
		m_nInLoseCount = atoi(Value);
	}else if(strcmp(Name,"direction") == 0){
		int _val = atoi(Value);
		if(_val >=1 ){
			m_bDirection = true;
		}else{
			m_bDirection = false;
		}
	}
	else if(strcmp(Name,"interval_inwin") == 0){
		m_fIntervalInWin = atof(Value);
	}else if(strcmp(Name,"interval_inlose") == 0){
		m_fIntervalInLose = atof(Value);
	}else if(strcmp(Name,"addpos_inwin") == 0){
		m_fAddPosInWin = atof(Value);
	}else if(strcmp(Name,"addpos_inlose") == 0){
		m_fAddPosInLose = atof(Value);
	}else if(strcmp(Name,"Paddenable_flag") == 0){
		this->m_nPositiveAddEnableFlag = atoi(Value);
	}else if(strcmp(Name,"Padded_flag") == 0){
		this->m_nPositiveAddedFlag = atoi(Value);
	}else if(strcmp(Name,"Naddenable_flag") == 0){
		this->m_nNegativeAddEnableFlag = atoi(Value);
	}else if(strcmp(Name,"Nadded_flag") == 0){
		this->m_nNegativeAddedFlag = atoi(Value);
	}else if(strcmp(Name,"current_stage") == 0){
		m_nCurrentStage = atoi(Value);
	}else if(strcmp(Name,"base_price_a") == 0){
		m_MyTradeA.m_fBasePrice = atof(Value);
	}else if(strcmp(Name,"base_price_b") == 0){
		m_MyTradeB.m_fBasePrice = atof(Value);
	}else if(strcmp(Name,"addinterval_step") == 0){
		m_nAddIntervalStep = atoi(Value);
		if(m_nAddIntervalStep < 1){
			m_nAddIntervalStep = 1;
		}
	}
	/*else if(strcmp(Name,"basepriceline") == 0){
		m_fBasePriceLine = atof(Value);
	}else if(strcmp(Name,"offsettoOpen") == 0){
		m_fOffsetToOpen = atof(Value);
	}else if(strcmp(Name,"offsettoClose") == 0){
		m_fOffsetToClose = atof(Value);
	}else if(strcmp(Name,"offsettoAdd1") == 0){
		m_fOffSetToAdd[0] = atof(Value);
	}else if(strcmp(Name,"offsettoAdd2") == 0){
		m_fOffSetToAdd[1] = atof(Value);
	}else if(strcmp(Name,"offsettoAdd3") == 0){
		m_fOffSetToAdd[2] = atof(Value);
	}else if(strcmp(Name,"postoAdd1") == 0){
		m_fPosToAdd[0] = atof(Value);
	}else if(strcmp(Name,"postoAdd2") == 0){
		m_fPosToAdd[1] = atof(Value);
	}else if(strcmp(Name,"postoAdd3") == 0){
		m_fPosToAdd[2] = atof(Value);
	}*/
	else if(strcmp(Name,"bartype") == 0){
		//m_nBarType
		int type = atoi(Value);
		if(type == 0){
			m_nBarType = Second;
		}
		else if(type == 1){
			m_nBarType = Minute;
		}
		else if(type == 2){
			m_nBarType = Hour;
		}
		else{
			m_nBarType = Day;
		}
	}else if(strcmp(Name,"barlength")==0){
		m_nBarLength = atoi(Value);
		cout<<"barLength : "<<m_nBarLength<<endl;
	}else if(strcmp(Name,"bartype_1") == 0){
		//m_nBarType
		int type = atoi(Value);
		if(type == 0){
			m_nBarType_1 = Second;
		}
		else if(type == 1){
			m_nBarType_1 = Minute;
		}
		else if(type == 2){
			m_nBarType_1 = Hour;
		}
		else{
			m_nBarType_1 = Day;
		}
	}else if(strcmp(Name,"barlength_1")==0){
		m_nBarLength_1 = atoi(Value);
	}
	else if(strcmp(Name,"opentimes") == 0){
		m_nInOpenTimes = atoi(Value);
	}else if(strcmp(Name,"totalSubtimes") == 0){
		m_nTotalSubTimes = atoi(Value);
	}else if(strcmp(Name,"totalAddtimes") == 0){
		m_nTotalAddTimes = atoi(Value);
	}else if(strcmp(Name,"baseaddtimes") == 0){
		m_nBaseAddTimes = atoi(Value);
	}



//****************************************************AAA
	else if(strcmp(Name,CLOSE_PROFIT) == 0){
		m_fCloseProfit = atof(Value);
	}else if(strcmp(Name,LONG_ADD_POS_A)==0){
		m_MyTradeA.m_nLongAddPos = atoi(Value);
	}else if(strcmp(Name,SHORT_ADD_POS_A) == 0){
		m_MyTradeA.m_nShortAddPos = atoi(Value);
	}else if(strcmp(Name,LONG_POS_A)==0){
		m_MyTradeA.m_nLongPos = atoi(Value);
	}else if(strcmp(Name,LONG_POS_PRICE_A)==0){
		m_MyTradeA.m_fAvgLongPrice = atof(Value);
	}else if(strcmp(Name,HIGH_SINCE_OPEN_LONG_A)==0){
		m_MyTradeA.m_fHighSinceOpenLong = atof(Value);
	}else if(strcmp(Name,LOW_SINCE_OPEN_LONG_A)==0){
		m_MyTradeA.m_fLowSinceOpenLong = atof(Value);
	}else if(strcmp(Name,SHORT_POS_A)==0){
		m_MyTradeA.m_nShortPos = atoi(Value);
	}else if(strcmp(Name,SHORT_POS_PRICE_A)==0){
		m_MyTradeA.m_fAvgShortPrice = atof(Value);
	}else if(strcmp(Name,HIGH_SINCE_OPEN_SHORT_A)==0){
		m_MyTradeA.m_fHighSinceOpenShort = atof(Value);
	}else if(strcmp(Name,LOW_SINCE_OPEN_SHORT_A)==0){
		m_MyTradeA.m_fLowSinceOpenShort = atof(Value);
	}
	//****************************************************BBB
	else if(strcmp(Name,LONG_ADD_POS_B)==0){
		m_MyTradeB.m_nLongAddPos = atoi(Value);
	}else if(strcmp(Name,SHORT_ADD_POS_B) == 0){
		m_MyTradeB.m_nShortAddPos = atoi(Value);
	}else if(strcmp(Name,LONG_POS_B)==0){
		m_MyTradeB.m_nLongPos = atoi(Value);
	}else if(strcmp(Name,LONG_POS_PRICE_B)==0){
		m_MyTradeB.m_fAvgLongPrice = atof(Value);
	}else if(strcmp(Name,HIGH_SINCE_OPEN_LONG_B)==0){
		m_MyTradeB.m_fHighSinceOpenLong = atof(Value);
	}else if(strcmp(Name,LOW_SINCE_OPEN_LONG_B)==0){
		m_MyTradeB.m_fLowSinceOpenLong = atof(Value);
	}else if(strcmp(Name,SHORT_POS_B)==0){
		m_MyTradeB.m_nShortPos = atoi(Value);
	}else if(strcmp(Name,SHORT_POS_PRICE_B)==0){
		m_MyTradeB.m_fAvgShortPrice = atof(Value);
	}else if(strcmp(Name,HIGH_SINCE_OPEN_SHORT_B)==0){
		m_MyTradeB.m_fHighSinceOpenShort = atof(Value);
	}else if(strcmp(Name,LOW_SINCE_OPEN_SHORT_B)==0){
		m_MyTradeB.m_fLowSinceOpenShort = atof(Value);
	}
	//**************************************************************
	//	m_nMATrend
}

void Z_DC_Spread_Policy::Release()
{
	delete this;
}

void Z_DC_Spread_Policy::ClosePos()
{
	cout<<"Close all pos Close_Force."<<endl;
	CloseAllPos();
	m_bStopOnClose=true;
}

void Z_DC_Spread_Policy::OnStoped(bool willDelete = false){
	cout<<"Z_DC_Spread_Policy stop......"<<endl;
}

void Z_DC_Spread_Policy::Init()
{
	//初始化200更接近真实值
	m_nHistoryCount = 90*15;
	//memset(&InsField,0,sizeof(FutureInstrumentField));
	strcpy(m_MyTradeA.InsField.InstrumentID, m_MyTradeA.m_InstrumentID);
	strcpy(m_MyTradeB.InsField.InstrumentID, m_MyTradeB.m_InstrumentID);
	if(!m_pTrader->GetInstrument(&m_MyTradeA.InsField)){
		throw "Invalid instrument_id A, please check.";
		Stop();
	}
	if(!m_pTrader->GetInstrument(&m_MyTradeB.InsField)){
		throw "Invalid instrument_id B, please check.";
		Stop();
	}
	if(strcmp(m_MyTradeA.m_InstrumentID,m_MyTradeB.m_InstrumentID) == 0){
		throw "The Same InstrumentID";
	}

	//ClosePos();
	cout<<"Price Tick: "<<m_MyTradeA.InsField.PriceTick<<" "<<m_MyTradeB.InsField.PriceTick<<" "<<m_MyTradeA.InsField.ExchangeID<<" "<<m_MyTradeB.InsField.ExchangeID<<" "<<strcmp(m_MyTradeB.InsField.ExchangeID,"SHFE")<<endl;
	//sleep(10);
	if(strcmp(m_MyTradeA.InsField.ExchangeID,"SHFE")==0){
		m_pTrader->SetSHFE(true);
		cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
	}if(strcmp(m_MyTradeB.InsField.ExchangeID,"SHFE") == 0){
		m_pTrader->SetSHFE(true);
		cout<<"**********************************"<<endl;
	}
	GetStartTime(m_MyTradeA.m_InstrumentID);
	//订阅行情
	SubscribeMarketData(m_MyTradeA.m_InstrumentID);
	SubscribeMarketData(m_MyTradeB.m_InstrumentID);

	m_MyTradeA.m_nMaxAddPosInWin = this->m_fAddPosInWin*m_MyTradeA.m_nMaxPos;
	m_MyTradeA.m_nMaxAddPosInLose = this->m_fAddPosInLose*m_MyTradeA.m_nMaxPos;
	m_MyTradeB.m_nMaxAddPosInWin = this->m_fAddPosInWin*m_MyTradeB.m_nMaxPos;
	m_MyTradeB.m_nMaxAddPosInLose = this->m_fAddPosInLose*m_MyTradeB.m_nMaxPos;

	cout<<"Inited and StartA:"<<m_MyTradeA.m_CountID<<" "<<m_MyTradeA.m_InstrumentID<<" "<<m_MyTradeA.m_nMaxPos<<" "<<m_MyTradeA.m_nMaxAddPosInWin<<" "<<m_MyTradeA.m_nMaxAddPosInLose<<endl;
	cout<<"Inited and StartB:"<<m_MyTradeB.m_CountID<<" "<<m_MyTradeB.m_InstrumentID<<" "<<m_MyTradeB.m_nMaxPos<<" "<<m_MyTradeB.m_nMaxAddPosInWin<<" "<<m_MyTradeB.m_nMaxAddPosInLose<<endl;

	m_MyTradeA.m_pSeries = m_pBarsCollection->CreateSeries(m_MyTradeA.m_InstrumentID,m_MyTradeA.InsField.ExchangeID,Minute,15,m_MyTradeA.InsField.VolumeMultiply,500);
	m_MyTradeA.m_pSeries->ReadHistoryLastN(10);
/*
	m_MyTradeA.m_pSeries_1 = m_pBarsCollection->CreateSeries(m_MyTradeA.m_InstrumentID,m_MyTradeA.InsField.ExchangeID,m_nBarType_1,m_nBarLength_1,10,2048);
	m_MyTradeA.m_pSeries_1->ReadHistoryLastN(1500);
	*/
	//m_pSeries = m_pBarsCollection->CreateSeries(m_InstrumentID, "CFFEX", m_nBarType,  m_nBarLength, 10, 2048);
	//m_pSeries->ReadHistoryLastN(m_nHistoryCount);
	//m_MyPUBU2 = new MyPUBU2(m_pSeries);


/*	m_MyTradeB.m_pSeries = m_pBarsCollection->CreateSeries(m_MyTradeB.m_InstrumentID,m_MyTradeB.InsField.ExchangeID,m_nBarType,m_nBarLength,10,2048);
	m_MyTradeB.m_pSeries->ReadHistoryLastN(1500);

	m_MyTradeB.m_pSeries_1 = m_pBarsCollection->CreateSeries(m_MyTradeB.m_InstrumentID,m_MyTradeB.InsField.ExchangeID,m_nBarType_1,m_nBarLength_1,10,2048);
	m_MyTradeB.m_pSeries_1->ReadHistoryLastN(1500);
	if(m_MyDCM != NULL){
		delete m_MyDCM;
	}
	if(m_MyDCM_1 != NULL){
		delete m_MyDCM_1;
	}
	m_MyDCM = new MyDCM(m_MyTradeA.m_pSeries,m_MyTradeB.m_pSeries,m_nN1,m_nN2,m_nN3);
	int _sizeA = m_MyTradeA.m_pSeries->m_nCount;
	int _sizeB = m_MyTradeB.m_pSeries->m_nCount;
	int _count = min(_sizeA, _sizeB);
	for(int i=_count; i>=2; i--){
		m_MyDCM->Update(_sizeA - i,_sizeB - i);
	}
	if(m_MyTradeA.m_pSeries->GetBar(_sizeA - 1)->m_fHighest != m_MyTradeA.m_pSeries->GetBar(_sizeA - 1)->m_fLowest ||
			m_MyTradeB.m_pSeries->GetBar(_sizeB - 1)->m_fHighest != m_MyTradeB.m_pSeries->GetBar(_sizeB - 1)->m_fLowest)
	{
		m_MyDCM->Update(_sizeA - 1,_sizeB - 1);
	}
	m_MyDCM_1 = new MyDCM(m_MyTradeA.m_pSeries_1,m_MyTradeB.m_pSeries_1,m_nN1,m_nN2,m_nN3);
	int _sizeA_1 = m_MyTradeA.m_pSeries_1->m_nCount;
	int _sizeB_1 = m_MyTradeB.m_pSeries_1->m_nCount;
	int _count_1 = min(_sizeA_1,_sizeB_1);
	for(int i=_count_1; i>=2; i--){
		m_MyDCM_1->Update(_sizeA_1 - i,_sizeB_1 - i);
	}
	if(m_MyTradeA.m_pSeries_1->GetBar(_sizeA_1 - 1)->m_fHighest != m_MyTradeA.m_pSeries_1->GetBar(_sizeA_1 - 1)->m_fLowest ||
			m_MyTradeB.m_pSeries_1->GetBar(_sizeB_1 - 1)->m_fHighest != m_MyTradeB.m_pSeries_1->GetBar(_sizeB_1 - 1)->m_fLowest)
	{
		m_MyDCM_1->Update(_sizeA_1 - 1,_sizeB_1 - 1);
	}


	cout<<m_MyTradeA.m_InstrumentID<<" init: "<<m_MyTradeA.m_pSeries->m_nCount<<" "<<m_MyDCM->m_fMA1<<" "<<m_MyDCM->m_fMA2<<" "<<m_MyDCM_1->m_fMA1<<" "<<m_MyDCM_1->m_fMA2<<endl;
	cout<<m_MyTradeB.m_InstrumentID<<" init: "<<m_MyTradeB.m_pSeries->m_nCount<<" "<<m_MyDCM->m_fMA1<<" "<<m_MyDCM->m_fMA2<<" "<<m_MyDCM_1->m_fMA1<<" "<<m_MyDCM_1->m_fMA2<<endl;
*/

	//初始化


	m_bStopOnClose = false;
	cout<<"Init end"<<endl;

}
void Z_DC_Spread_Policy::GetStartTime(char *ins){
	string tStr(ins);
	m_PrefixIns.clear();
	int _len=0;
	for(_len=0;_len<(int)tStr.length();_len++){
		if(tStr[_len]<='9'&&tStr[_len]>='0'){
			break;
		}
	}
	m_PrefixIns = tStr.substr(0,_len);
	if(m_PrefixIns == string("IF") || m_PrefixIns == string("IC") || m_PrefixIns == string("IH")){
		strcpy(m_cStartTime,"09:30:00");
	}else if(m_PrefixIns == string("TF") || m_PrefixIns == string("T")){
		strcpy(m_cStartTime,"09:15:00");
	}else{
		strcpy(m_cStartTime,"09:00:00");
	}
	cout<<"Start Time:"<<m_cStartTime<<" "<<tStr<<" "<<m_PrefixIns<<endl;//" EndTime: "<<m_cEndTimeList[m_PrefixIns]<<endl;
}
extern "C"{
	IPolicy* CreatePolicy(){
		return new Z_DC_Spread_Policy();
	}
}


