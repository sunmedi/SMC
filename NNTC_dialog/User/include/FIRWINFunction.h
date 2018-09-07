/*by Dr.Hwang 201705*/
#pragma once
#include <math.h>
#include <string.h>
#define TWO_PI	(2.0*PI)
#define HALF_OF_PI	(0.5*PI)
#define FFT_LEN_256	256
#define FFT_LEN_512	512
#define FFT_LEN_1024	1024
#define PI	3.1415926535898

#define F_LP	0		//LOW PASS
#define F_HP	1		//HIGH PASS
#define F_BP	2		//BAND PASS
#define F_BS	3		//BAND STOP

#define W_RECT		0	//RECT
#define W_TUKEY		1	//TUKEY
#define W_BARTLETT	2	//BARTLETT
#define W_HANN		3	//HANN
#define W_HAMMING	4	//HAMMING
#define W_BLACKMAN	5	//BLACKMAN
#define W_KAISER	6	//KAISER

void FirWin(double lf,double hf,double fs,int n,int baud,int windowtype,double H[]);
void Convolution(double Filter[],int Filter_n,double input[],int input_n,double result[]);
