/*
 * MyMACD.h
 *
 *  Created on: Mar 1, 2015
 *      Author: zhou
 */

#ifndef MYMACD_H_
#define MYMACD_H_
#include "qtapi.h"

class MyMACD {
public:
	MyMACD();
	MyMACD(IKBarSeries* pSeries,int Long,int Short,int m);
	virtual ~MyMACD();
	void Update(int index);
private:
	IKBarSeries* m_pSeries;
	int m_nLongCycle;
	int m_nShortCycle;
	int m_nM;
public:
	double m_fLongMA;
	double m_fShortMA;

	double m_fDIFF;
	double m_fDEA;
	double m_fMACD;
};

#endif /* MYMACD_H_ */
