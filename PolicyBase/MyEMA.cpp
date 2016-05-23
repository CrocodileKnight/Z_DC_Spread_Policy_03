/*
 * MyEMA.cpp
 *
 *  Created on: Mar 1, 2015
 *      Author: zhou
 */

#include "MyEMA.h"

MyEMA::MyEMA() {
	// TODO Auto-generated constructor stub

}

MyEMA::~MyEMA() {
	// TODO Auto-generated destructor stub
}

MyEMA::MyEMA(IKBarSeries* pSeries,int n){
	m_pSeries = pSeries;
	m_nN = n;
}

void MyEMA::Update(int index){

}
