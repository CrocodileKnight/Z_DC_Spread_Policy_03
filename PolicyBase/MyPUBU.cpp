/*
 * MyPUBU.cpp
 *
 *  Created on: Feb 5, 2015
 *      Author: zhou
 */

#include "MyPUBU.h"

MyPUBU::MyPUBU() {
	// TODO Auto-generated constructor stub

}

MyPUBU::~MyPUBU() {
	// TODO Auto-generated destructor stub
}

MyPUBU::MyPUBU(IKBarSeries* pSeries){
	m_pSeries = pSeries;
	m_nM[0] = 9;
	m_nM[1] = 12;
	m_nM[2] = 19;
	m_nM[3] = 27;
	m_nM[4] = 39;
	m_nM[5] = 49;
	memset(m_fPBX,0,sizeof(m_fPBX));
}

void MyPUBU::Update(int index){
	double _close = m_pSeries->GetBar(index)->m_fClose;
	if(index <= 1){
		m_fPBX[0] = _close;
		m_fPBX[1] = _close;
		m_fPBX[2] = _close;
		m_fPBX[3] = _close;
		m_fPBX[4] = _close;
		m_fPBX[5] = _close;
	}
	else{
		for(int i=0; i<6; i++)
			m_fPBX[i] = (2*_close + (m_nM[i]-1)*m_fPBX[i]) / (m_nM[i]+1);
	}
}
