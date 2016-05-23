/*
 * MyPUBU2.cpp
 *
 *  Created on: Mar 5, 2015
 *      Author: zhou
 */

#include "MyPUBU2.h"
#include <iostream>
using namespace std;

MyPUBU2::MyPUBU2() {
	// TODO Auto-generated constructor stub

}

MyPUBU2::~MyPUBU2() {
	// TODO Auto-generated destructor stub
}

MyPUBU2::MyPUBU2(IKBarSeries* pSeries){
	m_pSeries = pSeries;
	m_nM[0] = 4;
	m_nM[1] = 6;
	m_nM[2] = 9;
	m_nM[3] = 13;
	m_nM[4] = 18;
	m_nM[5] = 24;
	memset(m_fEMA,0,sizeof(m_fEMA));
	memset(m_fPBX,0,sizeof(m_fPBX));
}

void MyPUBU2::Update(int index){
	//int _count = m_pSeries->m_nCount;
	if(index<0 || m_pSeries == NULL)return;
	double _close = m_pSeries->GetBar(index)->m_fClose;
	if(index == 0){
		for(int i=0; i<6; i++){
			m_fEMA[i] = _close;
		}
		cout<<endl;
	}
	else{
		for(int i=0; i<6; i++){
			m_fEMA[i] = (_close*2 + (m_nM[i] - 1)*m_fEMA[i])/(m_nM[i] + 1);
		}
	}
	for(int i=0; i<6; i++){
		if(index+1 < m_nM[i]*4)
			return;
		m_fPBX[i] = GetPBX(i,index);
	}
}

double MyPUBU2::GetPBX(int n,int index){
	double _sum1 = 0;
	double _sum2 = 0;
	for(int i=index; i>=index+1 - m_nM[n]*4; i--){
		double _close = m_pSeries->GetBar(i)->m_fClose;
		if(index + 1 - i <= m_nM[n]*2)
			_sum1 += _close;
		_sum2 += _close;
	}
	_sum1 = _sum1 / (m_nM[n]*2);
	_sum2 = _sum2 / (m_nM[n]*4);
	double _result = (m_fEMA[n] + _sum1 + _sum2) / 3;
	return _result;
}









