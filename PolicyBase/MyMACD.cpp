/*
 * MyMACD.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: zhou
 */

#include "MyMACD.h"
#include<iostream>
using namespace std;

MyMACD::MyMACD() {
	// TODO Auto-generated constructor stub

}

MyMACD::~MyMACD() {
	// TODO Auto-generated destructor stub
}

MyMACD::MyMACD(IKBarSeries* pSeries,int Long,int Short,int m){
	m_pSeries = pSeries;
	m_nLongCycle = Long;
	m_nShortCycle = Short;
	m_nM = m;

	m_fDIFF = 0;
	m_fDEA = 0;
	m_fMACD = 0;
}

void MyMACD::Update(int index){
	if(index < 0) return;
	double _close = m_pSeries->GetBar(index)->m_fClose;
	if(index == 0){
		m_fLongMA = _close;
		m_fShortMA = _close;
		m_fDIFF = 0;
		m_fDEA = 0;
		m_fMACD = 0;
	}
	else if(index >= 1){
		m_fLongMA = (2*_close + (m_nLongCycle - 1)*m_fLongMA) / (m_nLongCycle + 1);
		m_fShortMA = (2*_close + (m_nShortCycle - 1)*m_fShortMA) / (m_nShortCycle + 1);
		m_fDIFF = m_fShortMA - m_fLongMA;
		m_fDEA = (2*m_fDIFF + (m_nM - 1)*m_fDEA) / (m_nM + 1);
		m_fMACD = 2*(m_fDIFF - m_fDEA);
	}
}
