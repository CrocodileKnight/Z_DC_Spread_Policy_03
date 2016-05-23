/*
 * MyRSI.h
 *
 *  Created on: Feb 9, 2015
 *      Author: zhou
 */

#ifndef MYRSI_H_
#define MYRSI_H_
#include <cmath>
#include "qtapi.h"

class MyRSI {
public:
	MyRSI();
	MyRSI(IKBarSeries* pSeries,int n);
	virtual ~MyRSI();
	void Update(int index);
private:
	IKBarSeries* m_pSeries;
	int m_nN;
	double m_fMax;
	double m_fAbs;
public:
	double m_fRSI;
};

#endif /* MYRSI_H_ */
