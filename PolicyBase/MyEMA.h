/*
 * MyEMA.h
 *
 *  Created on: Mar 1, 2015
 *      Author: zhou
 */

#ifndef MYEMA_H_
#define MYEMA_H_
#include "qtapi.h"


class MyEMA {
public:
	MyEMA();
	MyEMA(IKBarSeries* pSeries,int n);
	virtual ~MyEMA();
	void Update(int index);
private:
	IKBarSeries* m_pSeries;
	int m_nN;
public:
	double m_fMA;
};

#endif /* MYEMA_H_ */
