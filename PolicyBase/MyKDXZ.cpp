/*
 * MyKDXZ.cpp
 *
 *  Created on: Mar 7, 2015
 *      Author: zhou
 */

#include "MyKDXZ.h"
#include <iostream>
using namespace std;

MyKDXZ::MyKDXZ() {
	// TODO Auto-generated constructor stub

}

MyKDXZ::~MyKDXZ() {
	// TODO Auto-generated destructor stub
}

MyKDXZ::MyKDXZ(IKBarSeries* pSeries,int x,int y,int z){
	m_pSeries = pSeries;
	m_nX = x;
	m_nY = y;
	m_nZ = z;
	m_fRSV = 0;
	m_fTRSV = 0;
	m_fY0 = 0;
	m_fY1 = 0;
	m_fTY0 = 0;
	m_fTY1 = 0;
	printf("New KDXZ:%d , %d , %d\n",x,y,z);
	//cout<<"New KDXZ:"<<x<<" "<<y<<" "<<z<<endl;
}

/*
RSV:=(CLOSE-LLV(LOW,N2))/(HHV(HIGH,N2)-LLV(LOW,N2))*100,COLORWHITE,LINETHICK2;
Y0:SMA(RSV,N1,1),COLORRED,LINETHICK1;
Y1:SMA(Y0,N3,1),COLORYELLOW,LINETHICK1;
 *
 *
 */
void MyKDXZ::UpdateTick(){
	int _index = m_pSeries->m_nCount- 1;
	if(_index < 0) return;
	double _close = m_pSeries->GetBar(_index)->m_fClose;
	double _HHV = HHV(m_nX,false);
	double _LLV = LLV(m_nX,false);
	if(_HHV == _LLV) m_fTRSV = 100;
	else m_fTRSV = (_close - _LLV) / (_HHV - _LLV) * 100;
	m_fTY0 = (1*m_fTRSV + (m_nY - 1)*m_fY0) / m_nY;
	m_fTY1 = (1*m_fTY0 +  (m_nZ - 1)*m_fY1) / m_nZ;
}

void MyKDXZ::Update(int index){
	if(index < 0 ) return;
	double _close = m_pSeries->GetBar(index)->m_fClose;
	double _HHV = HHV(index,false);
	double _LLV = LLV(index,false);
	if(_HHV == _LLV) m_fRSV = 100;
	else m_fRSV = (_close - _LLV) / (_HHV - _LLV) * 100;
	if(index == 0){
		m_fY0 = m_fRSV;
		m_fY1 = m_fY0;
	}
	//			m_bMyBar[_count - 1].y0 = (1*m_bMyBar[_count - 1].rsv + (3-1)*m_bMyBar[_count-2].y0)/3; //this->SMA("rsv",3,1);
	else{
		m_fY0 = (1*m_fRSV + (m_nY - 1)*m_fY0) / m_nY;
		m_fY1 = (1*m_fY0 + (m_nZ - 1)*m_fY1) / m_nZ;
	}
}

double MyKDXZ::LLV(int index, bool isTick){
	int _length = min(m_nX,index);
	double _low = 9999999.0;
	for(int i=index; i>=index -_length; i--){
		double _value = m_pSeries->GetBar(i)->m_fLowest;
		if(_low > _value)
			_low = _value;
	}
	return _low;
}
double MyKDXZ::HHV(int index, bool isTick){
	int _length = min(m_nX,index);
	double _high = -9999999.0;
	for(int i=index; i>=index-_length; i--){
		double _value = m_pSeries->GetBar(i)->m_fHighest;
		if(_high<_value)
			_high = _value;
	}
	return _high;
}
