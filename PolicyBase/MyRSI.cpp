/*
 * MyRSI.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: zhou
 */

#include "MyRSI.h"

MyRSI::MyRSI() {
	// TODO Auto-generated constructor stub

}
MyRSI::MyRSI(IKBarSeries* pSeries,int n){
	m_pSeries = pSeries;
	m_nN = n;
	m_fRSI = 0.0;
}

MyRSI::~MyRSI() {
	// TODO Auto-generated destructor stub
}

void MyRSI::Update(int index){
	if(index <=0)return;
	else{
		double _val = m_pSeries->GetBar(index)->m_fClose - m_pSeries->GetBar(index-1)->m_fClose;
		double _max = _val > 0 ? _val : 0;
		double _abs = abs(_val);
		if(index == 1){
			m_fMax = _max;
			m_fAbs = _abs;
		}else{
			m_fMax = (_max + (m_nN - 1)*m_fMax) / m_nN;
			m_fAbs = (_abs + (m_nN - 1)*m_fAbs) / m_nN;
		}
		m_fRSI = m_fMax/m_fAbs * 100;
	}
}
