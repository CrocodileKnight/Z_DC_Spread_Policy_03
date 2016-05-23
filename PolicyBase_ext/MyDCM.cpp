/*
 * MyDCM.cpp
 *
 *  Created on: Apr 26, 2015
 *      Author: zhou
 */

#include "MyDCM.h"

MyDCM::MyDCM() {
	// TODO Auto-generated constructor stub

}

MyDCM::~MyDCM() {
	// TODO Auto-generated destructor stub
}

MyDCM::MyDCM(IKBarSeries* pSeries1,IKBarSeries* pSeries2,int x,int y,int z){
	m_pSeries1 = pSeries1;
	m_pSeries2 = pSeries2;
	N1 = x;
	N2 = y;
	N3 = z;
	m_fMA1 = 0;
	m_fMA2 = 0;
	m_pList.clear();
}

void MyDCM::Update(int index1,int index2){
	double _close1 = m_pSeries1->GetBar(index1)->m_fClose;
	double _close2 = m_pSeries2->GetBar(index2)->m_fClose;
	double _val = (_close1 - _close2) / _close1 * 1000;
	m_pList.push_back(_val);
	int _size = m_pList.size();
	if(_size < N1){
		return;
	}
	else{
		double _temp = m_pList[_size - N1];
		if(m_fMA1 == 0 && m_fMA2 == 0){
			m_fMA1 = m_fMA2 = _temp;
		}else{
			m_fMA1 = (_temp + (N2 - 1)*m_fMA1) / N2;
			m_fMA2 = (_temp + (N3 - 1)*m_fMA2) / N3;
		}
	}
//Y=[M*X+(N-M)*Y']/N
}
