/*
 * MyCCI.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: zhou
 */

#include "MyCCI.h"

MyCCI::MyCCI() {
	// TODO Auto-generated constructor stub

}
MyCCI::MyCCI(IKBarSeries* pSeries,int n){
	m_pSeries = pSeries;
	m_nN = n;
}

MyCCI::~MyCCI() {
	// TODO Auto-generated destructor stub
}

void MyCCI::Update(int index){
		if(index  < m_nN - 1){
			m_fCCI=0;
			return;
		};

		double *_typ = new double[m_nN];
		double _MAtyp = 0;
		double _AVEDEVtyp = 0;
		for(int i=0; i<m_nN; i++){
			IKBar *_bar = m_pSeries->GetBar(index - i);
			 _typ[m_nN-i-1] = (_bar->m_fClose + _bar->m_fHighest + _bar->m_fLowest) / 3.0;
			 _MAtyp += _typ[m_nN-i-1];
		}
		_MAtyp = _MAtyp / m_nN;
		for(int i=0; i<m_nN; i++){
			double _val = _typ[i] - _MAtyp;
			if(_val > 0){
				_AVEDEVtyp += _val;
			}else{
				_AVEDEVtyp -= _val;
			}
		}
		_AVEDEVtyp = _AVEDEVtyp / m_nN;
		double _result = (_typ[m_nN-1] - _MAtyp) / _AVEDEVtyp /0.015;
		m_fCCI = _result;
}

