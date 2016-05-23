/*
 * MyDCM.h
 *
 *  Created on: Apr 26, 2015
 *      Author: zhou
 */

#ifndef MYDCM_H_
#define MYDCM_H_
#include <vector>
#include "qtapi.h"


class MyDCM {
public:
	MyDCM();
	virtual ~MyDCM();
	MyDCM(IKBarSeries* pSeries1,IKBarSeries* pSeries2,int x,int y,int z);
	void Update(int index1,int index2);
private:
	int N1,N2,N3;
	IKBarSeries* m_pSeries1;
	IKBarSeries* m_pSeries2;
	vector<double> m_pList;
public:
	double m_fMA1;
	double m_fMA2;
};

#endif /* MYDCM_H_ */
