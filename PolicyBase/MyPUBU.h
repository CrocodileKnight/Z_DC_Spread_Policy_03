/*
 * MyPUBU.h
 *
 *  Created on: Feb 5, 2015
 *      Author: zhou
 */

#ifndef MYPUBU_H_
#define MYPUBU_H_
#include "qtapi.h"

class MyPUBU {
public:
	MyPUBU();
	MyPUBU(IKBarSeries* pSeries);
	virtual ~MyPUBU();
	void Update(int index);
private:
	IKBarSeries* m_pSeries;
	int m_nM[6];
public:
	double m_fPBX[6];
};

#endif /* MYPUBU_H_ */
