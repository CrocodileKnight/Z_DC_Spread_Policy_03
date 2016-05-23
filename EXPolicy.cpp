/*
 * EXPolicy.cpp
 *
 *  Created on: Dec 27, 2014
 *      Author: zhou
 */

#include "Z_DC_Spread_Policy.h"
bool Z_DC_Spread_Policy::IsTradeLimit(bool isAdd){
	if(isAdd){
		if(IsTradeLimit(&m_MyTradeA,true) || IsTradeLimit(&m_MyTradeB,false)){
			return true;
		}else{
			return false;
		}
	}else{
		if(IsTradeLimit(&m_MyTradeA,false) || IsTradeLimit(&m_MyTradeB,true)){
			return true;
		}else{
			return false;
		}
	}
}

bool Z_DC_Spread_Policy::IsTradeLimit(Member* member,bool IsSurged){
	if(IsSurged){
		//cout<<"up base:"<<member->m_InstrumentID<<" "<<member->m_pLastMD->AskPrice[0]<<" "<<member->m_pLastMD->BidPrice[0]<<" "<<member->m_pLastMD->UpperLimitPrice<<" "<<member->m_pLastMD->LowerLimitPrice<<endl;
		if(member->m_pLastMD->BidPrice[0] >= member->m_pLastMD->UpperLimitPrice - 0.00001 && member->m_pLastMD->BidPrice[0] <= member->m_pLastMD->UpperLimitPrice +0.00001){
			//cout<<member->m_InstrumentID<<" "<<member->m_pLastMD->LastPrice<<"up stop!"<<endl;
			return true;
		}else{
			return false;
		}
	}else{
		//cout<<"down base:"<<member->m_InstrumentID<<" "<<member->m_pLastMD->AskPrice[0]<<" "<<member->m_pLastMD->BidPrice[0]<<" "<<member->m_pLastMD->UpperLimitPrice<<" "<<member->m_pLastMD->LowerLimitPrice<<endl;
		if(member->m_pLastMD->AskPrice[0] <= member->m_pLastMD->LowerLimitPrice + 0.00001 && member->m_pLastMD->AskPrice[0] >= member->m_pLastMD->LowerLimitPrice - 0.00001){
			//cout<<member->m_InstrumentID<<" "<<member->m_pLastMD->LastPrice<<"down stop!"<<endl;
			return true;
		}else{
			return false;
		}
	}
}

void Z_DC_Spread_Policy::OrderChange(Member* member){
	if(member->m_pOrder->ErrorID !=0)
	{
		cout<<member->m_pLastMD->UpdateTime<<" "<<member->m_pOrder<<": Order Error!!! : "<<member->m_pOrder->ErrorID<<endl;
		member->m_pOrder = NULL;
		return;
	}

	if(member->m_pOrder == NULL){
		cout<<member->m_InstrumentID<<" Order is NULL "<<member->m_pOrder<<endl;
		return;
	}
	if(member->m_pOrder->OrderStatus == THOST_FTDC_OST_Unknown){
		return;
	}else if(member->m_pOrder->OrderStatus == THOST_FTDC_OST_Canceled){
		if(member->m_pOrder->nCust1<10){
			CatchOrder(member);
		}
		else{
			if(member->m_pOrder->Direction == THOST_FTDC_D_Buy && member->m_pOrder->OpenClose == THOST_FTDC_OF_Open){
				cout<< "Hedge Policy Buy Open Filled!!!"<<" "<<member->m_pOrder->InstrumentID<<" "<<member->m_pOrder->Price<<endl;
			}
			else if(member->m_pOrder->Direction == THOST_FTDC_D_Buy && member->m_pOrder->OpenClose != THOST_FTDC_OF_Open){
				cout<< "Hedge Policy Buy Close Filled!!!"<<" "<<member->m_pOrder->InstrumentID<<" "<<member->m_pOrder->Price<<endl;
			}
			else if(member->m_pOrder->Direction == THOST_FTDC_D_Sell && member->m_pOrder->OpenClose == THOST_FTDC_OF_Open){
				cout<< "Hedge Policy Sell Open Filled!!!"<<" "<<member->m_pOrder->InstrumentID<<" "<<member->m_pOrder->Price<<endl;
			}
			else if(member->m_pOrder->Direction == THOST_FTDC_D_Sell && member->m_pOrder->OpenClose != THOST_FTDC_OF_Open){
				cout<< "Hedge Policy Sell Close Filled!!!"<<" "<<member->m_pOrder->InstrumentID<<" "<<member->m_pOrder->Price<<endl;
			}
			member->m_pOrder = NULL;
		}
	}else if(member->m_pOrder->OrderStatus == THOST_FTDC_OST_AllTraded){
		member->m_pOrder = NULL;
		cout<<member->m_pLastMD->UpdateTime<<" "<<member->m_CountID<<"Order AllTraded"<<endl;
	}
	else{
		//cout<<"cancle order: "<<member->m_pOrder->nCust1<<" "<<member->m_pOrder->OrderStatus<<" "<< member->m_pOrder->ErrorID<<endl;
		if((member->m_pOrder->Direction == THOST_FTDC_D_Buy && member->m_pOrder->Price <= member->m_pLastMD->UpperLimitPrice)
				|| (member->m_pOrder->Direction == THOST_FTDC_D_Sell && member->m_pOrder->Price >= member->m_pLastMD->LowerLimitPrice)){
			if(member->m_pOrder->nCust1 >= 30){
				if(member->m_pOrder->Direction == THOST_FTDC_D_Buy && member->m_pOrder->OpenClose == THOST_FTDC_OF_Open){
					cout<< "Hedge Policy Buy Open Filled!!!"<<" "<<member->m_pOrder->InstrumentID<<" "<<member->m_pOrder->Price<<endl;
				}
				else if(member->m_pOrder->Direction == THOST_FTDC_D_Buy && member->m_pOrder->OpenClose != THOST_FTDC_OF_Open){
					cout<< "Hedge Policy Buy Close Filled!!!"<<" "<<member->m_pOrder->InstrumentID<<" "<<member->m_pOrder->Price<<endl;
				}
				else if(member->m_pOrder->Direction == THOST_FTDC_D_Sell && member->m_pOrder->OpenClose == THOST_FTDC_OF_Open){
					cout<< "Hedge Policy Sell Open Filled!!!"<<" "<<member->m_pOrder->InstrumentID<<" "<<member->m_pOrder->Price<<endl;
				}
				else if(member->m_pOrder->Direction == THOST_FTDC_D_Sell && member->m_pOrder->OpenClose != THOST_FTDC_OF_Open){
					cout<< "Hedge Policy Sell Close Filled!!!"<<" "<<member->m_pOrder->InstrumentID<<" "<<member->m_pOrder->Price<<endl;
				}
				member->m_pOrder = NULL;
			}else{
				m_pTrader->CancelOrder(member->m_pOrder);
			}
			//cout<<"cancelOrder..."<<endl;
		}
	}
}

void Z_DC_Spread_Policy::OrderFilled(Member *member,TradeField* pTrade){
	if(pTrade->Direction == THOST_FTDC_D_Buy){
		if(pTrade->OpenClose == THOST_FTDC_OF_Open){
			double fAmt = member->m_nLongPos * member->m_fAvgLongPrice;
			member->m_nLongPos += pTrade->Volume;
			member->m_fHighSinceOpenLong=pTrade->Price;
			member->m_fLowSinceOpenLong=pTrade->Price;
			member->m_fAvgLongPrice = (fAmt+pTrade->Volume * pTrade->Price)/member->m_nLongPos;
			cout<<"Long Open :"<<member->m_nLongPos<<" "<<member->m_fAvgLongPrice<<endl;
			SavePos(member, true);
		}
		else{
		//	double fAmt = m_nShortPos*m_fAvgShortPrice;
			CalCloseProfit(member,pTrade);

			member->m_nShortPos -= pTrade->Volume;
			if(member->m_nShortPos < 1){
				member->m_fAvgShortPrice = 0;
				member->m_fHighSinceOpenShort = 0;
				member->m_fLowSinceOpenShort = 0;
				member->m_nShortAddPos = 0;
			}

			//cout<<"will save Short"<<endl;
			SavePos(member, false);
		}
	}
	else{
		if(pTrade->OpenClose == THOST_FTDC_OF_Open){
			double fAmt = member->m_nShortPos * member->m_fAvgShortPrice;
			member->m_nShortPos += pTrade->Volume;
			member->m_fHighSinceOpenShort=pTrade->Price;
			member->m_fLowSinceOpenShort=pTrade->Price;
			member->m_fAvgShortPrice = (fAmt+pTrade->Volume * pTrade->Price) / member->m_nShortPos;
			cout<<"Short Open :"<<member->m_nShortPos<<" "<<member->m_fAvgShortPrice<<endl;
			SavePos(member, false);
		}
		else{
			//double fAmt = m_nLongPos*m_fAvgLongPrice;
			CalCloseProfit(member,pTrade);

			member->m_nLongPos -= pTrade->Volume;
			if(member->m_nLongPos < 1){
				member->m_fAvgLongPrice = 0;
				member->m_fHighSinceOpenLong = 0;
				member->m_fLowSinceOpenLong = 0;
				member->m_nLongAddPos = 0;
			}

			SavePos(member, true);
		}
	}
}
void Z_DC_Spread_Policy::CalCloseProfit(Member *member,TradeField* pTrade)
{
	char _buf[32];
	double _fProfit = 0.0;
	if(pTrade->Direction == THOST_FTDC_D_Sell && pTrade->OpenClose != THOST_FTDC_OF_Open){
		_fProfit = pTrade->Volume * (pTrade->Price - member->m_fAvgLongPrice) * member->InsField.VolumeMultiply;
	}
	if(pTrade->Direction == THOST_FTDC_D_Buy && pTrade->OpenClose != THOST_FTDC_OF_Open){
		_fProfit = pTrade->Volume * (member->m_fAvgShortPrice - pTrade->Price) * member->InsField.VolumeMultiply;
	}
	//cout<<"3m PUBU CalCloseProfit: "<<m_fAvgLongPrice<<" "<<m_fAvgShortPrice<<" "<<m_fCloseProfit<<" "<<_fProfit<<endl;
	m_fCloseProfit += _fProfit;
	//cout<<"3m PUBU CalCloseProfit: "<<m_fAvgLongPrice<<" "<<m_fAvgShortPrice<<" "<<m_fCloseProfit<<" "<<_fProfit<<endl;

	sprintf(_buf,"%.0f",m_fCloseProfit);
	this->UpdateCloseProfit(_buf);
}

void Z_DC_Spread_Policy::CatchOrder(Member* member){//OrderField* pOrder,MarketDataField* pMD
	double fPrice;
	OrderField* pNew = NULL;
	OrderField* pOrder = member->m_pOrder;
	MarketDataField* pMD = member->m_pLastMD;
	//cout<<"CatchOrder"<<endl;
	if(pOrder->Volume - pOrder->FilledVolume<1){
		return;
	}
	//cout<<"CatchOrder 1"<<endl;
	if(pOrder->Direction == THOST_FTDC_D_Buy){
		fPrice = pOrder->Price+ m_nSlipStep*member->InsField.PriceTick;
		if(fPrice>pMD->UpperLimitPrice){
			fPrice = pMD->UpperLimitPrice;
		}
		if(pOrder->OpenClose == THOST_FTDC_OF_Open){
			pNew = m_pTrader->Buy(pOrder->InstrumentID,pOrder->Volume-pOrder->FilledVolume,fPrice);
			cout<<"Retry Buy:"<<" "<<pOrder->Volume-pOrder->FilledVolume<<" "<<fPrice<<" "<<member->InsField.PriceTick<<endl;
		}else{
			pNew = m_pTrader->Cover(pOrder->InstrumentID,pOrder->Volume-pOrder->FilledVolume,fPrice);
			cout<<"Retry Cover:"<<" "<<pOrder->Volume-pOrder->FilledVolume<<" "<<fPrice<<" "<<member->InsField.PriceTick<<endl;
		}
	}else{
		fPrice = pOrder->Price-m_nSlipStep*member->InsField.PriceTick;
		if(fPrice < pMD->LowerLimitPrice){
			fPrice = pMD->LowerLimitPrice;
		}
		if(pOrder->OpenClose == THOST_FTDC_OF_Open){
			pNew = m_pTrader->Short(pOrder->InstrumentID,pOrder->Volume-pOrder->FilledVolume,fPrice);
			cout<<"Retry Short:"<<" "<<pOrder->Volume-pOrder->FilledVolume<<" "<<fPrice<<" "<<member->InsField.PriceTick<<endl;
		}else{
			//cout<<"Catch Order"<<endl;
			pNew = m_pTrader->Sell(pOrder->InstrumentID,pOrder->Volume-pOrder->FilledVolume,fPrice);
			cout<<"Retry Sell:"<<" "<<pOrder->Volume-pOrder->FilledVolume<<" "<<fPrice<<" "<<member->InsField.PriceTick<<endl;
		}
	}
	if(pNew){
		pNew->nCust1 = pOrder->nCust1+1;
	}
	member->m_pOrder = pNew;
//	return pNew;
}
int Z_DC_Spread_Policy::GetCurrentBit(double winRate){//+30级，-30级
	int x = 0;
	if(winRate > 0)
	{
		x = winRate/m_fIntervalInWin;
		if(x>=MaxBit)x = MaxBit;
	}else{
		x = winRate / -m_fIntervalInLose;
		if(x>=MaxBit)x = MaxBit;
	}
	return x;
}
void Z_DC_Spread_Policy::ManageRateSingal(){
	if(m_MyTradeA.m_pOrder != NULL || m_MyTradeB.m_pOrder != NULL){
		return;
	}
	double _cWinRate = (this->GetBaseWinRate(&m_MyTradeA) + this->GetBaseWinRate(&m_MyTradeB))*100;
	int _cBit = this->GetCurrentBit(_cWinRate);

	if(_cWinRate > 0){
		int _cBitToAdd = _cBit - m_nAddIntervalStep -1;
		if(_cBitToAdd>=0){
			int _temp = m_nPositiveAddEnableFlag;
			this->SetBit(m_nPositiveAddEnableFlag,_cBitToAdd);
			if(_temp!=m_nPositiveAddEnableFlag){
				SetAttr("Paddenable_flag",m_nPositiveAddEnableFlag);
			}


			int _sCount = 0;
			int _ssCount = 0;
			if(!IsTradeLimit(false)){
				for(int i=_cBitToAdd;i>=0;i--){  		//正向减仓数
					if(GetBit(m_nPositiveAddedFlag,i)){
						ClearBit(m_nPositiveAddedFlag,i);
						SetBit(m_nPositiveAddEnableFlag,i);
						_sCount++;
					}
				}
				for(int i=0;i<=MaxBit;i++){				//负向减仓数
					if(GetBit(m_nNegativeAddedFlag,i)){
						ClearBit(m_nNegativeAddedFlag,i);
						SetBit(m_nNegativeAddEnableFlag,i);
						_ssCount++;
					}
				}
			}
			//判断是否有仓位需要减仓
			if(_sCount>0 || _ssCount>0){
				int _aVolToSub = m_MyTradeA.m_nMaxAddPosInLose*_ssCount + m_MyTradeA.m_nMaxAddPosInWin*_sCount;
				int _bVolToSub = m_MyTradeB.m_nMaxAddPosInLose*_ssCount + m_MyTradeB.m_nMaxAddPosInWin*_sCount;
				if(!GetBit(m_nPositiveAddedFlag,_cBit) && GetBit(m_nPositiveAddEnableFlag,_cBit)){
					SetBit(m_nPositiveAddedFlag,_cBit);
					ClearBit(m_nPositiveAddEnableFlag,_cBit);
					int _aVolToAdd = m_MyTradeA.m_nMaxAddPosInWin;
					int _bVolToAdd = m_MyTradeB.m_nMaxAddPosInWin;
					if(_aVolToAdd<_aVolToSub){
						SubtractLong(&m_MyTradeA,_aVolToSub - _aVolToAdd);
						SubtractShort(&m_MyTradeB,_bVolToSub - _bVolToAdd);
						this->m_nSubTimesInDay++;
						this->m_nTotalSubTimes++;
						SetAttr("totalSubtimes",m_nTotalSubTimes);
					}else{
						AddLong(&m_MyTradeA,_aVolToAdd - _aVolToSub);
						AddShort(&m_MyTradeB,_bVolToAdd - _bVolToSub);
						this->m_nAddTimesInDay++;
						this->m_nTotalAddTimes++;
						SetAttr("totalAddtimes",m_nTotalAddTimes);

					}
					this->ShowInfo(&m_MyTradeA,"Positive Exchange Bit To SubTract and Add 1:",_sCount);
					this->ShowInfo(&m_MyTradeA,"Positive Exchange Bit To SubTract and Add 2:",_ssCount);
					cout<<"Exchange Bit"<<_cBit<<" "<<m_nPositiveAddEnableFlag<<" "<<m_nPositiveAddedFlag<<endl;
				}
				else{
					SubtractLong(&m_MyTradeA,_aVolToSub);
					SubtractShort(&m_MyTradeB,_bVolToSub);
					this->m_nSubTimesInDay++;
					this->m_nTotalSubTimes++;
					SetAttr("totalSubtimes",m_nTotalSubTimes);

					this->ShowInfo(&m_MyTradeA,"Positive To SubTract 1:",_sCount);
					this->ShowInfo(&m_MyTradeA,"Positive To SubTract 2:",_ssCount);
					cout<<"Win Subtract :"<<_cBit<<" "<<m_nPositiveAddEnableFlag<<" "<<m_nPositiveAddedFlag<<endl;
				}
				SetAttr("Padded_flag",m_nPositiveAddedFlag);
				SetAttr("Paddenable_flag",m_nPositiveAddEnableFlag);
			}
			//设置加仓位
			else{
				if(!IsTradeLimit(true) && !GetBit(m_nPositiveAddedFlag,_cBit) && GetBit(m_nPositiveAddEnableFlag,_cBit)){
						AddLong(&m_MyTradeA,true);
						AddShort(&m_MyTradeB,true);
						this->m_nAddTimesInDay++;
						this->m_nTotalAddTimes++;
						SetAttr("totalAddtimes",m_nTotalAddTimes);

						SetBit(m_nPositiveAddedFlag,_cBit);
						ClearBit(m_nPositiveAddEnableFlag,_cBit);
						SetAttr("Padded_flag",m_nPositiveAddedFlag);
						SetAttr("Paddenable_flag",m_nPositiveAddEnableFlag);
						this->ShowInfo(&m_MyTradeA,"Win Add :",_cBit);
						cout<<"Win Add :"<<m_nPositiveAddEnableFlag<<" "<<m_nPositiveAddedFlag<<endl;
				}
			}
		}
		else{//正负交接处!!!!!!!!!!!!!!
			int _NcBitToAdd = m_nAddIntervalStep - _cBit;
			if(_NcBitToAdd > MaxBit)_NcBitToAdd = MaxBit;
			int _temp = m_nNegativeAddEnableFlag;
			this->SetBit(m_nNegativeAddEnableFlag,_NcBitToAdd);
			if(_temp != m_nNegativeAddEnableFlag){
				SetAttr("Naddenable_flag",m_nNegativeAddEnableFlag);
			}
			//判断是否有仓位需要减仓
			int _ssCount = 0;
			if(!IsTradeLimit(false)){
				for(int i=_NcBitToAdd;i<=MaxBit;i++){
					if(GetBit(m_nNegativeAddedFlag,i)){
						ClearBit(m_nNegativeAddedFlag,i);
						SetBit(m_nNegativeAddEnableFlag,i);
						_ssCount++;
					}
				}
			}
			if(_ssCount>0){
				int _aVolToSub = m_MyTradeA.m_nMaxAddPosInLose*_ssCount;
				int _bVolToSub = m_MyTradeB.m_nMaxAddPosInLose*_ssCount;
				if(!GetBit(m_nPositiveAddedFlag,_cBit) && GetBit(m_nPositiveAddEnableFlag,_cBit)){
					int _aVolToAdd = m_MyTradeA.m_nMaxAddPosInWin;
					int _bVolToAdd = m_MyTradeB.m_nMaxAddPosInWin;
					if(_aVolToAdd<_aVolToSub){
						SubtractLong(&m_MyTradeA,_aVolToSub - _aVolToAdd);
						SubtractShort(&m_MyTradeB,_bVolToSub - _bVolToAdd);
						this->m_nSubTimesInDay++;
						this->m_nTotalSubTimes++;
						SetAttr("totalSubtimes",m_nTotalSubTimes);
					}else{
						AddLong(&m_MyTradeA,_aVolToAdd - _aVolToSub);
						AddShort(&m_MyTradeB,_bVolToAdd - _bVolToSub);
						this->m_nAddTimesInDay++;
						this->m_nTotalAddTimes++;
						SetAttr("totalAddtimes",m_nTotalAddTimes);
					}
					SetBit(m_nPositiveAddedFlag,_cBit);
					ClearBit(m_nPositiveAddEnableFlag,_cBit);
					SetAttr("Padded_flag",m_nPositiveAddedFlag);
					SetAttr("Paddenable_flag",m_nPositiveAddEnableFlag);
					this->ShowInfo(&m_MyTradeA,"Positive Exchange Bit To SubTract and Add 3:",_ssCount);
					//this->ShowInfo(&m_MyTradeA,"Positive Exchange Bit To SubTract and Add 2:",_ssCount);
					cout<<"Exchange Bit"<<_cBit<<" "<<m_nPositiveAddEnableFlag<<" "<<m_nPositiveAddedFlag<<endl;
				}
				else{
					SubtractLong(&m_MyTradeA,_aVolToSub);
					SubtractShort(&m_MyTradeB,_bVolToSub);
					this->m_nSubTimesInDay++;
					this->m_nTotalSubTimes++;
					SetAttr("totalSubtimes",m_nTotalSubTimes);

					this->ShowInfo(&m_MyTradeA,"Lose Subtract :",_ssCount);
					cout<<"Lose Subtract :"<<_cBit<<" "<<m_nNegativeAddEnableFlag<<" "<<m_nNegativeAddedFlag<<endl;
				}
				SetAttr("Nadded_flag",m_nNegativeAddedFlag);
				SetAttr("Naddenable_flag",m_nNegativeAddEnableFlag);
			}
			//设置加仓位
			else{
				if(!IsTradeLimit(true) && !GetBit(m_nPositiveAddedFlag,_cBit) && GetBit(m_nPositiveAddEnableFlag,_cBit)){
						AddLong(&m_MyTradeA,true);
						AddShort(&m_MyTradeB,true);
						this->m_nAddTimesInDay++;
						this->m_nTotalAddTimes++;
						SetAttr("totalAddtimes",m_nTotalAddTimes);
						this->ShowInfo(&m_MyTradeA,"Win Add :",_cBit);
						SetBit(m_nPositiveAddedFlag,_cBit);
						ClearBit(m_nPositiveAddEnableFlag,_cBit);
						SetAttr("Padded_flag",m_nPositiveAddedFlag);
						SetAttr("Paddenable_flag",m_nPositiveAddEnableFlag);
						cout<<"Win Add :"<<m_nPositiveAddEnableFlag<<" "<<m_nPositiveAddedFlag<<endl;
				}
			}
		}

	}else{
		int _cBitToAdd = _cBit + m_nAddIntervalStep + 1;
		if(_cBitToAdd >= MaxBit)_cBitToAdd = MaxBit;
		int _temp = m_nNegativeAddEnableFlag;
		this->SetBit(m_nNegativeAddEnableFlag,_cBitToAdd);
		if(_temp!=m_nNegativeAddEnableFlag){
			SetAttr("Naddenable_flag",m_nNegativeAddEnableFlag);
		}
		//判断是否有仓位需要减仓
		//计算当前位置有多少仓位需要减掉，防止跳空高开
		int _ssCount = 0;
		if(!IsTradeLimit(false)){
			for(int i=_cBitToAdd;i<=MaxBit;i++){
				if(GetBit(m_nNegativeAddedFlag,i)){
					ClearBit(m_nNegativeAddedFlag,i);
					SetBit(m_nNegativeAddEnableFlag,i);
					_ssCount++;
				}
			}
		}
		if(_ssCount>0){
			if(!GetBit(m_nNegativeAddedFlag,_cBit) && GetBit(m_nNegativeAddEnableFlag,_cBit)){
				//交换加仓位标记，不发平仓开仓命令
				SetBit(m_nNegativeAddedFlag,_cBit);
				ClearBit(m_nNegativeAddEnableFlag,_cBit);
				if(_ssCount > 1){
					SubtractLong(&m_MyTradeA,m_MyTradeA.m_nMaxAddPosInLose*(_ssCount - 1));
					SubtractShort(&m_MyTradeB,m_MyTradeB.m_nMaxAddPosInLose*(_ssCount - 1));
					this->m_nSubTimesInDay++;
					this->m_nTotalSubTimes++;
					SetAttr("totalSubtimes",m_nTotalSubTimes);
				}
				this->ShowInfo(&m_MyTradeA,"Negative Exchange Bit To SubTract and Add",_ssCount - 1);
				cout<<"Exchange Bit"<<_cBit<<" "<<m_nNegativeAddEnableFlag<<" "<<m_nNegativeAddedFlag<<endl;
			}
			else{
				SubtractLong(&m_MyTradeA,m_MyTradeA.m_nMaxAddPosInLose*_ssCount);
				SubtractShort(&m_MyTradeB,m_MyTradeB.m_nMaxAddPosInLose*_ssCount);
				this->m_nSubTimesInDay++;
				this->m_nTotalSubTimes++;
				SetAttr("totalSubtimes",m_nTotalSubTimes);
				this->ShowInfo(&m_MyTradeA,"Lose Subtract :",_ssCount);
				cout<<"Lose Subtract :"<<_cBit<<" "<<m_nNegativeAddEnableFlag<<" "<<m_nNegativeAddedFlag<<endl;
			}
			SetAttr("Nadded_flag",m_nNegativeAddedFlag);
			SetAttr("Naddenable_flag",m_nNegativeAddEnableFlag);
		}
		//设置加仓位
		else{
			if(!IsTradeLimit(true) && !GetBit(m_nNegativeAddedFlag,_cBit) && GetBit(m_nNegativeAddEnableFlag,_cBit)){
					AddLong(&m_MyTradeA,false);
					AddShort(&m_MyTradeB,false);
					this->m_nAddTimesInDay++;
					this->m_nTotalAddTimes++;
					SetAttr("totalAddtimes",m_nTotalAddTimes);
					SetBit(m_nNegativeAddedFlag,_cBit);
					ClearBit(m_nNegativeAddEnableFlag,_cBit);
					SetAttr("Nadded_flag",m_nNegativeAddedFlag);
					SetAttr("Naddenable_flag",m_nNegativeAddEnableFlag);
					this->ShowInfo(&m_MyTradeA,"Lose Add :",_cBit);
					cout<<"Lose Add :"<<m_nNegativeAddEnableFlag<<" "<<m_nNegativeAddedFlag<<endl;
			}
		}
	}
}
double Z_DC_Spread_Policy::GetBaseWinRate(const Member* member){
	double _fProfit = 0;
	if(member->m_nLongPos > 0 && member->m_fBasePrice !=0){
		_fProfit += (member->m_pLastMD->LastPrice - member->m_fBasePrice)/member->m_fBasePrice;
	}else if(member->m_nShortPos>0 && member->m_fBasePrice!=0){
		_fProfit += (member->m_fBasePrice - member->m_pLastMD->LastPrice)/member->m_fBasePrice;
	}
	return _fProfit;
}
double Z_DC_Spread_Policy::GetWinPoint(const Member* member){
	double fEarn =0;
	if(member->m_nLongPos > 0){
			fEarn += member->m_pLastMD->LastPrice - member->m_fAvgLongPrice;
	}else if(member->m_nShortPos >0){
			fEarn += member->m_fAvgShortPrice - member->m_pLastMD->LastPrice;
	}
	return fEarn;
}

double Z_DC_Spread_Policy::GetProfit(const Member* member){
	double fEarn =0;
	double fProfit=0;
	if(member->m_nLongPos > 0){
		fEarn = member->m_pLastMD->LastPrice - member->m_fAvgLongPrice;
		fProfit = member->m_nLongPos * fEarn * member->InsField.VolumeMultiply;
		//fMaxEarn = member->m_fHighSinceOpenLong - member->m_fAvgLongPrice;
		//fMaxProfit = member->m_nLongPos * fMaxEarn * member->InsField.VolumeMultiply;
	}else if(member->m_nShortPos > 0){
		fEarn = member->m_fAvgShortPrice - member->m_pLastMD->LastPrice;
		fProfit = member->m_nShortPos * fEarn * member->InsField.VolumeMultiply;
		//fMaxEarn = member->m_fAvgShortPrice - member->m_fLowSinceOpenShort;
		//fMaxProfit = member->m_nShortPos * fMaxEarn * member->InsField.VolumeMultiply;
	}
	return fProfit;
}

void Z_DC_Spread_Policy::CalProfit(){
	char cBuf[128];
	double _fProfitA = GetProfit(&m_MyTradeA);
	double _fProfitB = GetProfit(&m_MyTradeB);

	m_fCurrentProfit = _fProfitA + _fProfitB;
	if(m_fMaxProfit < m_fCurrentProfit){
		m_fMaxProfit = m_fCurrentProfit;
	}
	double _fWithdraw = m_fMaxProfit - m_fCurrentProfit;
	//cout<<_fProfitA<<" "<<_fProfitB<<" "<<m_fCurrentProfit<<" "<<m_fMaxProfit<<" "<<_fWithdraw<<endl;

	//double _fEarnA = GetWinPoint(&m_MyTradeA);
	//double _fEarnB = GetWinPoint(&m_MyTradeB);
	//double _fEarn = _fEarnA + _fEarnB;
	//cout<<_fProfitA<<" "<<_fProfitB<<" "<<m_fCurrentProfit<<" "<<m_fMaxProfit<<" "<<_fWithdraw<<" "<<_fEarn<<endl;

	if(m_fMaxProfit > this->m_fStopWinBase && _fWithdraw/m_fMaxProfit > this->m_fStopWinFactor){
		CloseAllPos();
		m_fMaxProfit = -9999999;
		cout<<"stop win :"<<m_fCurrentProfit<<endl;
	}
	if(m_fCurrentProfit < -m_fStopLoseBase)
	{
		CloseAllPos();
		m_fMaxProfit = -9999999;
		cout<<"stop lose :"<<m_fCurrentProfit<<endl;
	}

	updateTicks++;
	if(updateTicks%2==0){
		sprintf(cBuf,"%.0f",m_fCurrentProfit);
		this->UpdatePosProfit(cBuf);
		double _wA = m_MyTradeA.m_nLongPos  * m_MyTradeA.InsField.VolumeMultiply * m_MyTradeA.m_pLastMD->LastPrice * 0.01;
		double _vA = 10000 / (m_MyTradeA.InsField.VolumeMultiply * m_MyTradeA.m_pLastMD->LastPrice * 0.01);
		double _wB = m_MyTradeB.m_nShortPos * m_MyTradeB.InsField.VolumeMultiply * m_MyTradeB.m_pLastMD->LastPrice * 0.01;
		double _vB = 10000 / (m_MyTradeB.InsField.VolumeMultiply * m_MyTradeB.m_pLastMD->LastPrice * 0.01);
		sprintf(cBuf,"%.0f/%.1f%%___%.1f:%.1f_%.1f:%.1f",_fWithdraw,m_fMaxProfit>0.001?_fWithdraw/m_fMaxProfit*100:100,_wA,_wB,_vA,_vB);
		this->UpdateWithDraw(cBuf);

		if(updateTicks%4==0){
			sprintf(cBuf,"%i/%i__%i/%i",m_nAddTimesInDay,m_nTotalAddTimes,m_nSubTimesInDay,m_nTotalSubTimes);
			SetAttr(STATUSMSG,cBuf,true);
		}else
		{
			double _cWinRate = (this->GetBaseWinRate(&m_MyTradeA) + this->GetBaseWinRate(&m_MyTradeB)) * 100;
			int _cBit = this->GetCurrentBit(_cWinRate);
			sprintf(cBuf,"%.2f_%i_%i_%i__%i,%i",_cWinRate,_cBit,this->m_nPositiveAddEnableFlag,this->m_nPositiveAddedFlag,this->m_nNegativeAddEnableFlag,this->m_nNegativeAddedFlag);
			SetAttr(STATUSMSG,cBuf,true);
		}
		if(updateTicks>128){
			updateTicks=1;
		}
	}
}

void Z_DC_Spread_Policy::CloseAllPos(){
	CloseShort(&m_MyTradeB);
	CloseLong(&m_MyTradeB);
	CloseLong(&m_MyTradeA);
	CloseShort(&m_MyTradeA);

}

double Z_DC_Spread_Policy::GetPriceForBuy(const Member *member){
	double _fPrice = 0;
	if((int)member->m_pLastMD->AskPrice[0] == 0){
		_fPrice = member->m_pLastMD->UpperLimitPrice;
	}
	else{
		_fPrice = member->m_pLastMD->AskPrice[0] + m_nSlipStep*member->InsField.PriceTick;
		if(_fPrice > 9999999.0 || _fPrice > member->m_pLastMD->UpperLimitPrice)
			_fPrice = member->m_pLastMD->UpperLimitPrice;
	}
	return _fPrice;
}
double Z_DC_Spread_Policy::GetPriceForSell(const Member *member){
	double _fPrice = 0;
	if((int)member->m_pLastMD->BidPrice[0] == 0){
		_fPrice = member->m_pLastMD->LowerLimitPrice;
	}
	else{
		_fPrice = member->m_pLastMD->BidPrice[0] - m_nSlipStep*member->InsField.PriceTick;
		if(_fPrice < 0 || _fPrice < member->m_pLastMD->LowerLimitPrice)
			_fPrice = member->m_pLastMD->LowerLimitPrice;
	}
	return _fPrice;
}
void Z_DC_Spread_Policy::OpenLong(Member *member){
	//cout<<member->m_InstrumentID<<" "<<member->m_nMaxPos<<endl;
	if(member->m_pOrder != NULL){
		return;
	}
	if(member->m_nLongPos + member->m_nShortPos < member->m_nMaxPos){
		int nVol = member->m_nMaxPos-member->m_nLongPos - member->m_nShortPos;
		if(nVol>0){
			double fPrice = GetPriceForBuy(member);
			member->m_pOrder = m_pTrader->Buy(member->m_InstrumentID,nVol,fPrice);
			cout<<member->m_CountID<<" "<<member->m_pLastMD->UpdateTime<<" Buy :"<<member->m_InstrumentID<<" "<<nVol<<" "<<fPrice<<endl;
		}
	}
}
void Z_DC_Spread_Policy::OpenShort(Member *member){
	if(member->m_pOrder != NULL){
		return;
	}
	if(member->m_nShortPos + member->m_nLongPos < member->m_nMaxPos){
		int nVol = member->m_nMaxPos-member->m_nShortPos - member->m_nLongPos;
		if(nVol>0){
			double fPrice = GetPriceForSell(member);
			member->m_pOrder = m_pTrader->Short(member->m_InstrumentID,nVol,fPrice);
			cout<<member->m_CountID<<" "<<member->m_pLastMD->UpdateTime<<" Short :"<<member->m_InstrumentID<<" "<<nVol<<" "<<fPrice<<endl;
		}
	}
}
void Z_DC_Spread_Policy::AddLong(Member *member,bool isWin){
	if(member->m_pOrder != NULL){
		return;
	}
	int nVol = 0;
	if(isWin){
		nVol = member->m_nMaxAddPosInWin;
	}else{
		nVol = member->m_nMaxAddPosInLose;
	}
	//int _MaxPosToOpen = member->m_nMaxPos*(1+m_fPosToAdd[0]+m_fPosToAdd[1]+m_fPosToAdd[2]);
	if(nVol>0){
		double fPrice = GetPriceForBuy(member);
		member->m_pOrder = m_pTrader->Buy(member->m_InstrumentID,nVol,fPrice);
		this->ShowInfo(member,"AddLong",nVol);
		//cout<<"Add Long:"<<" "<<nVol<<" "<<fPrice<<endl;
	}
}
void Z_DC_Spread_Policy::AddLong(Member* member,int vol){
	if(member->m_pOrder != NULL){
		return;
	}
	int nVol = vol;
	if(nVol>0){
		double fPrice = GetPriceForBuy(member);
		member->m_pOrder = m_pTrader->Buy(member->m_InstrumentID,nVol,fPrice);
		this->ShowInfo(member,"AddLong By Vol:",nVol);
		//cout<<"Add Long:"<<" "<<nVol<<" "<<fPrice<<endl;
	}
}
void Z_DC_Spread_Policy::AddShort(Member *member,bool isWin){
	if(member->m_pOrder != NULL){
		return;
	}
	//cout<<"Add Short"<<endl;
	int nVol = 0;
	if(isWin){
		nVol = member->m_nMaxAddPosInWin;
	}else{
		nVol = member->m_nMaxAddPosInLose;
	}
	//int
	if(nVol>0){
		double fPrice = GetPriceForSell(member);
		member->m_pOrder = m_pTrader->Short(member->m_InstrumentID,nVol,fPrice);
		this->ShowInfo(member,"AddShort",nVol);

	}

}
void Z_DC_Spread_Policy::AddShort(Member *member,int vol){
	if(member->m_pOrder != NULL){
		return;
	}
	int nVol = vol;
	//int
	if(nVol>0){
		double fPrice = GetPriceForSell(member);
		member->m_pOrder = m_pTrader->Short(member->m_InstrumentID,nVol,fPrice);
		this->ShowInfo(member,"AddShort By Vol:",nVol);
	}
}
void Z_DC_Spread_Policy::CloseLong(Member *member){
	if(member->m_pOrder || member->m_nLongPos<1 || member->m_pLastMD == NULL){
		cout<<"CloseLong no run!"<<endl;
		return;
	}

	int nClosable=0,nTotal =0,nTodayClosable;
	m_pTrader->GetLongPos(member->m_InstrumentID,nTotal,nClosable,nTodayClosable);
	if(nTotal<1){
		member->m_nLongPos=0;
		member->m_fAvgLongPrice=0;
		member->m_fHighSinceOpenLong =0;
		member->m_fLowSinceOpenLong =0;
		this->SavePos(member, true);
	}else{
		int nVol = min(member->m_nLongPos,nClosable);
		if(nVol>0){
			double fPrice = GetPriceForSell(member);
			member->m_pOrder = m_pTrader->Sell(member->m_InstrumentID,nVol,fPrice);
			cout<<"Sell :"<<member->m_InstrumentID<<" "<<nVol<<" "<<fPrice<<endl;
		}
	}
}
void Z_DC_Spread_Policy::CloseShort(Member *member){
	if(member->m_pOrder || member->m_nShortPos<1 || member->m_pLastMD == NULL){
		cout<<"CloseShort no run!"<<endl;
		return;
	}

	int nClosable=0,nTotal =0,nTodayClosable;
	m_pTrader->GetShortPos(member->m_InstrumentID,nTotal,nClosable,nTodayClosable);
	if(nTotal<1){
		member->m_nShortPos=0;
		member->m_fAvgShortPrice=0;
		member->m_fHighSinceOpenShort =0;
		member->m_fLowSinceOpenShort =0;

		this->SavePos(member, false);
	}else{
		int nVol = min(member->m_nShortPos,nClosable);
		if(nVol>0){
			double fPrice = GetPriceForBuy(member);
			member->m_pOrder = m_pTrader->Cover(member->m_InstrumentID,nVol,fPrice);
			cout<<"Cover :"<<member->m_InstrumentID<<" "<<nVol<<" "<<fPrice<<endl;
		}
	}
}
void Z_DC_Spread_Policy::SubtractLong(Member* member,bool isWin){
	//if()
	if(member->m_pOrder || member->m_nLongPos<1){
		return;
	}
	int _closepos = 0;
	if(isWin){
		_closepos = member->m_nMaxAddPosInWin;
	}else{
		_closepos = member->m_nMaxAddPosInLose;
	}

	int nClosable=0,nTotal =0,nTodayClosable;
	m_pTrader->GetLongPos(member->m_InstrumentID,nTotal,nClosable,nTodayClosable);
	if(nTotal<1){
		member->m_nLongPos=0;
		member->m_fAvgLongPrice=0;
		member->m_fHighSinceOpenLong =0;
		member->m_fLowSinceOpenLong =0;
		this->SavePos(member,true);

	}else{
		int nVol = min(_closepos,nClosable);
		if(nVol>0){
			double fPrice = this->GetPriceForSell(member);
			member->m_pOrder = m_pTrader->Sell(member->m_InstrumentID,nVol,fPrice);
			this->ShowInfo(member,"subtract long:",nVol);
		}
	}
}
void Z_DC_Spread_Policy::SubtractLong(Member* member,int vol){
	if(member->m_pOrder || member->m_nLongPos<1){
		return;
	}
	int _closepos = vol;
	int nClosable=0,nTotal =0,nTodayClosable;
	m_pTrader->GetLongPos(member->m_InstrumentID,nTotal,nClosable,nTodayClosable);
	if(nTotal<1){
		member->m_nLongPos=0;
		member->m_fAvgLongPrice=0;
		member->m_fHighSinceOpenLong =0;
		member->m_fLowSinceOpenLong =0;
		this->SavePos(member,true);

	}else{
		int nVol = min(_closepos,nClosable);
		if(nVol>0){
			double fPrice = this->GetPriceForSell(member);
			member->m_pOrder = m_pTrader->Sell(member->m_InstrumentID,nVol,fPrice);
			this->ShowInfo(member,"subtract long by vol:",nVol);
		}
	}
}
void Z_DC_Spread_Policy::SubtractShort(Member* member,bool isWin){
	if(member->m_pOrder || member->m_nShortPos<1){
		return;
	}
	int _closepos = 0;
	if(isWin){
		_closepos = member->m_nMaxAddPosInWin;
	}else{
		_closepos = member->m_nMaxAddPosInLose;
	}
	int nClosable=0,nTotal =0,nTodayClosable;
	m_pTrader->GetShortPos(member->m_InstrumentID,nTotal,nClosable,nTodayClosable);
	if(nTotal<1){
		member->m_nShortPos=0;
		member->m_fAvgShortPrice=0;
		member->m_fHighSinceOpenShort =0;
		member->m_fLowSinceOpenShort =0;

		this->SavePos(member,false);
	}else{
		int nVol = min(_closepos,nClosable);
		if(nVol>0){
			double fPrice = GetPriceForBuy(member);
			member->m_pOrder = m_pTrader->Cover(member->m_InstrumentID,nVol,fPrice);
			this->ShowInfo(member,"subtract short:",nVol);
		}
	}
}
void Z_DC_Spread_Policy::SubtractShort(Member* member,int vol){
	if(member->m_pOrder || member->m_nShortPos<1){
		return;
	}
	int _closepos = vol;
	int nClosable=0,nTotal =0,nTodayClosable;
	m_pTrader->GetShortPos(member->m_InstrumentID,nTotal,nClosable,nTodayClosable);
	if(nTotal<1){
		member->m_nShortPos=0;
		member->m_fAvgShortPrice=0;
		member->m_fHighSinceOpenShort =0;
		member->m_fLowSinceOpenShort =0;
		this->SavePos(member,false);
	}else{
		int nVol = min(_closepos,nClosable);
		if(nVol>0){
			double fPrice = GetPriceForBuy(member);
			member->m_pOrder = m_pTrader->Cover(member->m_InstrumentID,nVol,fPrice);
			this->ShowInfo(member,"subtract short by vol:",nVol);
		}
	}
}

void Z_DC_Spread_Policy::UpdateMD(MarketDataField* pMD){
	if(strcmp(pMD->InstrumentID, m_MyTradeA.m_InstrumentID) == 0){
		m_MyTradeA.m_pLastMD = pMD;
		if(m_MyTradeA.m_fHighSinceOpenLong>0.0001 && pMD->LastPrice>m_MyTradeA.m_fHighSinceOpenLong){
			m_MyTradeA.m_fHighSinceOpenLong = pMD->LastPrice;
		}
		if(m_MyTradeA.m_fLowSinceOpenLong>0.0001 && pMD->LastPrice<m_MyTradeA.m_fLowSinceOpenLong){
			m_MyTradeA.m_fLowSinceOpenLong = pMD->LastPrice;
		}

		//high low price since open short position;
		if(m_MyTradeA.m_fHighSinceOpenShort>0.0001 && pMD->LastPrice>m_MyTradeA.m_fHighSinceOpenShort){
			m_MyTradeA.m_fHighSinceOpenShort = pMD->LastPrice;
		}
		if(m_MyTradeA.m_fLowSinceOpenShort>0.0001 && pMD->LastPrice<m_MyTradeA.m_fLowSinceOpenShort){
			m_MyTradeA.m_fLowSinceOpenShort = pMD->LastPrice;
		}
	}
	if(strcmp(pMD->InstrumentID, m_MyTradeB.m_InstrumentID) == 0){
		m_MyTradeB.m_pLastMD = pMD;
		if(m_MyTradeB.m_fHighSinceOpenLong>0.0001 && pMD->LastPrice>m_MyTradeB.m_fHighSinceOpenLong){
			m_MyTradeB.m_fHighSinceOpenLong = pMD->LastPrice;
		}
		if(m_MyTradeB.m_fLowSinceOpenLong>0.0001 && pMD->LastPrice<m_MyTradeB.m_fLowSinceOpenLong){
			m_MyTradeB.m_fLowSinceOpenLong = pMD->LastPrice;
		}

		//high low price since open short position;
		if(m_MyTradeB.m_fHighSinceOpenShort>0.0001 && pMD->LastPrice>m_MyTradeB.m_fHighSinceOpenShort){
			m_MyTradeB.m_fHighSinceOpenShort = pMD->LastPrice;
		}
		if(m_MyTradeB.m_fLowSinceOpenShort>0.0001 && pMD->LastPrice<m_MyTradeB.m_fLowSinceOpenShort){
			m_MyTradeB.m_fLowSinceOpenShort = pMD->LastPrice;
		}
	}

	//high low price since open long position;
/*
	//double _changeprice;
	if(m_nLongPos > 0){
		if(m_pLastBar->m_fClose - pMD->LastPrice > m_fMaxLosePointInBars)
		{
			CloseLong(Close_StopLose);
		}
	}
	if(m_nShortPos > 0){
		if(pMD->LastPrice - m_pLastBar->m_fClose > m_fMaxLosePointInBars)
		{
			//cout<<"close short stop lose"<<endl;
			CloseShort(Close_StopLose);
		}

	}*/
}


int Z_DC_Spread_Policy::SetBit(int &Value,int index){

	Value = Value|(1<<index);
	return Value;
}
int Z_DC_Spread_Policy::ClearBit(int &Value,int index){
	Value = Value& (~(1<<index));
	return Value;
}
int Z_DC_Spread_Policy::GetBit(const int Value,int index){
	int x = (Value>>index) & 1;
	return x;
}

void Z_DC_Spread_Policy::ShowInfo(Member* member,const char* buf,double value){
	if(member->m_pLastMD!=NULL){
		cout<<member->m_pLastMD->TradingDay<<" "<<member->m_pLastMD->UpdateTime<<" "<<member->m_CountID<<" "<<member->m_InstrumentID<<" "<<buf<<" "<<value<<endl;
	}else{
		cout<<"WithOut pMD: "<<buf<<endl;
	}
}
/*
void Z_DC_Spread_Policy::AddLong(){

	if(m_pOrder){
		//cout<<"OpenLong Faild"<<endl;
		return;
	}
	//cout<<"Add Long......"<<endl;
	if(m_nShortAddPos + m_nLongAddPos < m_nMaxAddPos){
		int nVol = m_nMaxAddPos - m_nLongAddPos - m_nShortAddPos;
		if(nVol>0){
			double fPrice;
			fPrice = m_pLastMD->AskPrice[0]+m_nSlipStep*InsField.PriceTick;

			if(m_pLastMD->AskPrice[0]>999999999.0){
				fPrice = m_pLastMD->UpperLimitPrice;
			}else{
				fPrice = m_pLastMD->AskPrice[0]+m_nSlipStep*InsField.PriceTick;
				if(fPrice >m_pLastMD->UpperLimitPrice){
					fPrice = m_pLastMD->UpperLimitPrice;
				}
			}
			m_pOrder = m_pTrader->Buy(m_InstrumentID,nVol,fPrice);
			m_pTradeState = Trade_BuyingAdd;
			//cout<<"Add Long:"<<" "<<nVol<<" "<<fPrice<<endl;
		}
	}
}
void Z_DC_Spread_Policy::AddShort(){
	if(m_pOrder){
		return;
	}
	//cout<<"Add Short"<<endl;
	if(m_nShortAddPos + m_nLongAddPos < m_nMaxAddPos){
		int nVol = m_nMaxAddPos - m_nLongAddPos - m_nShortAddPos;
		if(nVol>0){
			double fPrice = m_pLastMD->BidPrice[0]-m_nSlipStep*InsField.PriceTick;
			if(fPrice >9999999999.0 || fPrice<m_pLastMD->LowerLimitPrice){
				fPrice = m_pLastMD->LowerLimitPrice;
			}

			m_pOrder = m_pTrader->Short(m_InstrumentID,nVol,fPrice);
			//cout<<"Add Short:"<<" "<<nVol<<" "<<fPrice<<endl;
			m_pTradeState = Trade_ShortingAdd;
		}
	}
}
void Z_DC_Spread_Policy::SubtractLong(bool isAll){
	//if()
	if(m_pOrder || m_nLongPos<1 || m_nLongAddPos<1){
		return;
	}
	int _closepos = m_nLongAddPos;

	int nClosable=0,nTotal =0,nTodayClosable;
	m_pTrader->GetLongPos(m_InstrumentID,nTotal,nClosable,nTodayClosable);
	if(nTotal<1){
		m_nLongPos=0;
		m_fAvgLongPrice=0;
		m_fHighSinceOpenLong =0;
		m_fLowSinceOpenLong =0;
		this->SavePos(true);

	}else{
		int nVol = min(_closepos,nClosable);
		if(nVol>0){
			double fPrice = m_pLastMD->BidPrice[0]-m_nSlipStep*InsField.PriceTick;
			if(fPrice >9999999999.0 || fPrice<m_pLastMD->LowerLimitPrice){
				fPrice = m_pLastMD->LowerLimitPrice;
			}
			m_pOrder = m_pTrader->Sell(m_InstrumentID,nVol,fPrice);
			if(m_pLastMD->LastPrice < m_fAvgLongPrice){
				m_nCurrentLoseCount++;
			}
			//cout<<"Sell:"<<" "<<nVol<<" "<<fPrice<<" "<<endl;
			m_pTradeState = Trade_Selling;
			m_pCloseState = Close_SubtractPos;

		}
	}
}
void Z_DC_Spread_Policy::SubtractShort(bool isAll){
	if(m_pOrder || m_nShortPos<1 || m_nShortAddPos<1){
		return;
	}
	int _closepos = m_nShortAddPos;

	int nClosable=0,nTotal =0,nTodayClosable;
	m_pTrader->GetShortPos(m_InstrumentID,nTotal,nClosable,nTodayClosable);
	if(nTotal<1){
		m_nShortPos=0;
		m_fAvgShortPrice=0;
		m_fHighSinceOpenShort =0;
		m_fLowSinceOpenShort =0;

		this->SavePos(false);
	}else{
		int nVol = min(_closepos,nClosable);
		if(nVol>0){
			double fPrice;
			if(m_pLastMD->AskPrice[0]>999999999.0){
				fPrice = m_pLastMD->UpperLimitPrice;
			}else{
				fPrice = m_pLastMD->AskPrice[0]+m_nSlipStep*InsField.PriceTick;
				if(fPrice >m_pLastMD->UpperLimitPrice){
					fPrice = m_pLastMD->UpperLimitPrice;
				}
			}
			m_pOrder = m_pTrader->Cover(m_InstrumentID,nVol,fPrice);
			if(m_pLastMD->LastPrice > m_fAvgShortPrice){
				m_nCurrentLoseCount++;
			}
			m_pTradeState = Trade_Coverring;
			m_pCloseState = Close_SubtractPos;
		}
	}
}
*/


void Z_DC_Spread_Policy::UpdatePosProfit(char *buf){
	SetAttr(POS_PROFIT,buf,true);
}
void Z_DC_Spread_Policy::UpdateWithDraw(char *buf){
	SetAttr(WITHDRAW,buf,true);
}
void Z_DC_Spread_Policy::UpdateCloseProfit(char *buf){
	SetAttr(CLOSE_PROFIT,buf,true);
}

void Z_DC_Spread_Policy::SavePos(const Member* member, bool isLongPos){
	//cout<<"save pos: "<<member->m_InstrumentID<<" "<<isLongPos<<" "<<m_MyTradeA.m_InstrumentID<<" "<<m_MyTradeB.m_InstrumentID<<endl;
	if(isLongPos){
		//cout<<"shit: 1"<<member->m_InstrumentID<<" "<<m_MyTradeA.m_InstrumentID<<" "<<m_MyTradeB.m_InstrumentID<<endl;
		if(strcmp(member->m_InstrumentID, m_MyTradeA.m_InstrumentID) == 0){
			//cout<<"................."<<endl;
			SetAttr(LONG_ADD_POS_A,member->m_nLongAddPos,true);
			SetAttr(LONG_POS_A,member->m_nLongPos,true);
			SetAttr(LONGPOS,member->m_nLongPos,true);
			SetAttr(LONG_POS_PRICE_A,member->m_fAvgLongPrice);
			SetAttr(HIGH_SINCE_OPEN_LONG_A,member->m_fHighSinceOpenLong);
			SetAttr(LOW_SINCE_OPEN_LONG_A,member->m_fLowSinceOpenLong);
			//cout<<"Save Long"<<endl;
		}
		if(strcmp(member->m_InstrumentID, m_MyTradeB.m_InstrumentID) == 0){
			//cout<<"*********************"<<endl;
			SetAttr(LONG_ADD_POS_B,member->m_nLongAddPos,true);
			SetAttr(LONG_POS_B,member->m_nLongPos,true);
			SetAttr(LONGPOS,member->m_nLongPos,true);
			SetAttr(LONG_POS_PRICE_B,member->m_fAvgLongPrice);
			SetAttr(HIGH_SINCE_OPEN_LONG_B,member->m_fHighSinceOpenLong);
			SetAttr(LOW_SINCE_OPEN_LONG_B,member->m_fLowSinceOpenLong);

		}
	}
	else{
		//cout<<"shit: 2"<<member->m_InstrumentID<<" "<<m_MyTradeA.m_InstrumentID<<" "<<m_MyTradeB.m_InstrumentID<<endl;
		if(strcmp(member->m_InstrumentID, m_MyTradeA.m_InstrumentID) == 0){
			//cout<<"-------------------"<<endl;
			SetAttr(SHORT_ADD_POS_A,member->m_nShortAddPos,true);
			SetAttr(SHORT_POS_A,member->m_nShortPos,true);
			SetAttr(SHORTPOS,member->m_nShortPos,true);
			SetAttr(SHORT_POS_PRICE_A,member->m_fAvgShortPrice);
			SetAttr(HIGH_SINCE_OPEN_SHORT_A,member->m_fHighSinceOpenShort);
			SetAttr(LOW_SINCE_OPEN_SHORT_A,member->m_fLowSinceOpenShort);
		}
		if(strcmp(member->m_InstrumentID, m_MyTradeB.m_InstrumentID) == 0){
			//cout<<"++++++++++++++++++"<<endl;
			SetAttr(SHORT_ADD_POS_B,member->m_nShortAddPos,true);
			SetAttr(SHORT_POS_B,member->m_nShortPos,true);
			SetAttr(SHORTPOS,member->m_nShortPos,true);
			SetAttr(SHORT_POS_PRICE_B,member->m_fAvgShortPrice);
			SetAttr(HIGH_SINCE_OPEN_SHORT_B,member->m_fHighSinceOpenShort);
			SetAttr(LOW_SINCE_OPEN_SHORT_B,member->m_fLowSinceOpenShort);
		}
	}
}
