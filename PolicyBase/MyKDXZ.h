/*
 * MyKDXZ.h
 *
 *  Created on: Mar 7, 2015
 *      Author: zhou
 */

#ifndef MYKDXZ_H_
#define MYKDXZ_H_
#include "qtapi.h"

class MyKDXZ {
public:
	MyKDXZ();
	MyKDXZ(IKBarSeries* pSeries,int x,int y,int z);
	virtual ~MyKDXZ();
	void Update(int index);
	void UpdateTick();
private:
	double LLV(int n,bool isTick);
	double HHV(int n,bool isTick);
private:
	IKBarSeries* m_pSeries;
	int m_nX;
	int m_nY;
	int m_nZ;
	double m_fRSV;
	double m_fTRSV; //Tick
public:
	double m_fY0;
	double m_fY1;
	double m_fTY0;  //Tick
	double m_fTY1;	//Tick
};
/*
RSV:=(CLOSE-LLV(LOW,N2))/(HHV(HIGH,N2)-LLV(LOW,N2))*100,COLORWHITE,LINETHICK2; X
Y0:SMA(RSV,N1,1),COLORRED,LINETHICK1; Y
Y1:SMA(Y0,N1,1),COLORYELLOW,LINETHICK1; Z
 */

#endif /* MYKDXZ_H_ */
