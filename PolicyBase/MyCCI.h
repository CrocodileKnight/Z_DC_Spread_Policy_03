/*
 * MyCCI.h
 *
 *  Created on: Feb 10, 2015
 *      Author: zhou
 */

#ifndef MYCCI_H_
#define MYCCI_H_
#include "qtapi.h"

class MyCCI {
public:
	MyCCI();
	MyCCI(IKBarSeries* pSeries,int n);
	void Update(int index);
	virtual ~MyCCI();
private:
	IKBarSeries* m_pSeries;
	int m_nN;
public:
	double m_fCCI;
};

#endif /* MYCCI_H_ */
