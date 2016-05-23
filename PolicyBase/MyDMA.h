/*
 * MyDMA.h
 *
 *  Created on: Apr 10, 2015
 *      Author: zhou
 */

#ifndef MYDMA_H_
#define MYDMA_H_
#include "qtapi.h"
#include <deque>

class MyDMA {
public:
	MyDMA();
	virtual ~MyDMA();
	void Update(int index);
	MyDMA(IKBarSeries* pSeries, int m, int n,int s);
private:
	IKBarSeries* m_pSeries;
	int m_nN1;
	int m_nN2;
	int m_nN3;
	deque<int> m_DArray;
public:
	double m_fDDD;
	double m_fAMA;
};

#endif /* MYDMA_H_ */
