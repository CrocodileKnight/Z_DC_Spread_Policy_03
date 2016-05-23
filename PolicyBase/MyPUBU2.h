/*
 * MyPUBU2.h
 *
 *  Created on: Mar 5, 2015
 *      Author: zhou
 */

#ifndef MYPUBU2_H_
#define MYPUBU2_H_
#include "qtapi.h"

class MyPUBU2 {
public:
	MyPUBU2();
	MyPUBU2(IKBarSeries *pSeries);
	virtual ~MyPUBU2();
	void Update(int index);
	double GetPBX(int n,int index);
private:
	IKBarSeries* m_pSeries;
	int m_nM[6];
	double m_fEMA[6];
public:
	double m_fPBX[6];
};

#endif /* MYPUBU2_H_ */
