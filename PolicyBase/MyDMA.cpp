/*
 * MyDMA.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: zhou
 */

#include "MyDMA.h"

MyDMA::MyDMA() {
	// TODO Auto-generated constructor stub

}

MyDMA::~MyDMA() {
	// TODO Auto-generated destructor stub
}

MyDMA::MyDMA(IKBarSeries* pSeries, int m, int n, int s){
	m_pSeries = pSeries;
	m_nN1 = m;
	m_nN2 = n;
	m_nN3 = s;
	m_fDDD = 0;
	m_fAMA = 0;
}

void MyDMA::Update(int index){
	double _short = 0;
	double _long = 0;
	if(index >= m_nN1 - 1){
		double _sum = 0;
		for(int i=index; i>= index-m_nN1+1; i--){
			_sum += m_pSeries->GetBar(i)->m_fClose;
		}
		_short = _sum / m_nN1;
	}
	if(index >= m_nN2 - 1){
		double _sum = 0;
		for(int i=index; i>=index-m_nN2+1; i--){
			_sum += m_pSeries->GetBar(i)->m_fClose;
		}
		_long = _sum / m_nN2;
	}
	if(_short !=0 && _long != 0){
		m_fDDD = _short - _long;
		m_DArray.push_back(m_fDDD);
		if(m_DArray.size() > m_nN3){
			m_DArray.pop_front();
		}
		double _sum = 0;
		for(int i=0; i<m_DArray.size(); i++){
			_sum += m_DArray[i];
		}
		m_fAMA = _sum/m_DArray.size();
	}
}

