#ifndef DFilterController
#define DFilterController
#define ARRAY_DIM 50
struct TIIRCoeff
{
	double a0[ARRAY_DIM]; double a1[ARRAY_DIM]; double a2[ARRAY_DIM]; double a3[ARRAY_DIM]; double a4[ARRAY_DIM];
	double b0[ARRAY_DIM]; double b1[ARRAY_DIM]; double b2[ARRAY_DIM]; double b3[ARRAY_DIM]; double b4[ARRAY_DIM];
	int NumSections;
};

class FilterController
{
public:
	FilterController(){};
	~FilterController(){};
	
	TIIRCoeff IIRCOEFF;

	void ComputIIRFilter(int PassType,int IIRType,double OmegaC,double BandWidth);
	void IIRFilterProcessing(double *Signal, double *FilteredSignal, int Numofdata);
};

#endif