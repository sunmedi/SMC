#pragma once
#include <math.h>
#include "..\User\include\fftw3.h"
//#define PROCESSING_FFT_BUF_SIZE 256
#define PROCESSING_FFT_BUF_SIZE 2048
#define Pi 3.1415926535898
struct Point2D
{
	float x;
	float y;
};
class Spectrum
{
public:
	Spectrum(void);
	~Spectrum(void);
	void DestroySpectrum();
	void InitSpectrum(int SamplesHz,string SensorPosName_,int width_,int height_);
	void ResetSpectrum();
	void SetSpectrumOrg(float x,float y);
	void UpdateSpectrum();
	void UpdateSpectrum256();
	void FFTProcessing(double Y_Array_R[],int currentDataFrameInx_FFT);//just for recode play
	void FFTProcessing3(vector<double>Y_Array);
	void FFTProcessing2(vector<double>Y_Array);//compute for recoded data/input data  in one time
	void FFTProcessingDAQ(vector<double>Y_Array_R);
	void FFTProcessing256(vector<double>Y_Array_R);
	void DisplayPowerSpectrum();
	void DisplayAmplitudeSpectrum();
	void DisplayPowerSpectrum2();//2 chanel
	void DisplayAmplitudeSpectrum2();//2 chanel

	void DisplayPowerSpectrumTLC();
	void DisplayAmplitudeSpectrumTLC();
	bool IsChanel2;

	void drawString(const char* str);
	void FFTProcessing4(double Y_Array[],int c_idx);//46080 for TLC
	void FFTProcessing256TLC(double Y_Array[],int c_idx);
	void UpdateSpectrum256TLC();

	//void FFTProcessingALLTLC(double Y_Array[],int c_idx);
	//void UpdateSpectrumALLTLC();

	void HammingWin(int windowLength, float *buffer);
public:
	float HammingWin256[256];
	float HammingWin2048[2048];
	//double max_yyyy;
	Point2D org;
	//vector<double>X_Array;
	//vector<double>Y_Array_PS;//PS
	//vector<double>Y_Array_AS;//AS
	double X_Array_50[50];
	double Y_Array_AS_50[50];
	double Y_Array_Phase_50[50];
	double Y_Array_PS_50[50];

	double SumOfPS;//for relative map
	double SumOfAS;

	/*double Max_AS_detail;
	double Min_AS_detail;
	double Max_AS_standard;
	double Min_AS_standard;*/

	double all_sum;//0-49
	double derta_sum;//0-4
	double theta_sum;//5-7
	double alpha_sum;//8-13
	double beta_sum;//14-30
	double gamma_sum;//31-49

	double Low_sum;//default 4-7
	double Reward_sum;// 12-15
	double High_sum;// 22-36

	string SensorPosName;
	int Samples_Hz;//x 
	double in_array_double[PROCESSING_FFT_BUF_SIZE];
	double in_array_double_256[256];
	/*double in_array_double_128[128];
	double in_array_double_64[64];
	double in_array_double_32[32];
	double in_array_double_16[16];
	double in_array_double_8[8];*/

	fftw_complex *out_array,*out_array256;//*out_array8,*out_array16,*out_array32,*out_array64,*out_array128;
	fftw_plan fft_p,fft_p256;//fft_p8,fft_p16,fft_p32,fft_p64,fft_p128;

	bool IsFull2048;
	double width;
	double height;

	double Low_success;
	double Reward_success;
	double High_success;

	int Low_TCount;
	int Low_SCount;
	int Rew_TCount;
	int Rew_SCount;
	int Hig_TCount;
	int Hig_SCount;
	


};
