#pragma once
#include "Iir.h"

class IIRProc
{
public:
	IIRProc(void);
	void initFilter(int samplingrate,float center,float width,double Ripple);
	double FilterProcess(double input_);
	~IIRProc(void);
	//Iir::Elliptic::BandPass<order,Iir::DirectFormI> bp;
	Iir::Elliptic::BandPass<7,Iir::DirectFormI> bp;//for our filter use order 7
};
