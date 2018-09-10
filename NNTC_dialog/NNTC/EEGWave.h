#pragma once
#define OFFSET_ 5
#define DURATION_ 4
#define RewardAMPOffset 2.0  
#define BUF_SIZE 32
#define FIR_TAP 64
#include <vector>
#include "UsbThread.h"
#include "..\User\include\iir\IIRProc.h"

using namespace std;

struct FIRProcDataStruct
{
	double lf;
	double hf;
	double fs;
	int FIR_LEN;
	int Filter_type;
	int Window_type;
	int Window_width;
};
struct IIRProcDataStruct
{
	int PassType;//iirLPF, iirHPF, iirBPF, iirNOTCH, iirALLPASS 
	int IIRType;//BUTTERWORTH;CHEBYSHEV;INVERSE_CHEBY;ELLIPTIC;
	double OmegaC;
	double BandWidth;
};
struct WaveDataStruct
{
	double vol;
	bool IsOverFlowT;
};

/**
*
* @class EEGWave
* @brief 데이터 FFT 계산
* @details 디지털 값으로 변환된 값을 Filter를 이용하여 원하는 값으로 만든다. 
* @author 황해도
* @date 2018-09-05
* @version 0.0.1
*
*/
class EEGWave
{
public:
	
	bool NewHalfAuto1();
	bool NewHalfAuto2();
	bool NewHalfAuto3();

	EEGWave(void);
	~EEGWave(void);

	void InitValue(bool issinglec,char* name_, double ThreshValue_,double width,double Org_Y,bool IsRawWave_,IIRProcDataStruct iirDataSet_,bool Isinhibit_,double scal_v_,double TargetOverPercent_,double Absolute_Thresh_,double offset_Target,int Duration_for_auto_,double RawThreshValueBuf_);

	void RenderWave();
	void RenderWave_2();
	void UpdateWave(double rawwave[],bool isRecoding,double FFT_AS[]);
	void UpdateWave_2(double rawwave[],double rawwave2[],bool isRecoding,double FFT_AS[],double FFT_AS2[]);
	bool GetRawDataFromUSB(bool isRecoding);//RUNNING IN THREAD
	bool GetRawDataFromUSB_2(bool isRecoding);//RUNNING IN THREAD
	void TurnPage();
	bool OpenUsbComm();
	void CloseUsbComm();
	void start();
	void pause();
	void resume();
	void stop();
	void DrawString(const char* str);
	void ReSetFIR();
	void ReSetIIR();
	void HalfAuto();
	float PreHalfAuto();
	double MaxAMPInRange(int s_,int range);
	void ReProcess8secData();
	float ReProcess8secData_1(double ThreshValue_);//for min value method
	float ReProcess8secData_2(double ThreshValue_);//for min value method
	float ReProcess8secData_3(double ThreshValue_);//for min value method
	void ReSetXArray(double width,double Org_Y);
	void ReSetDataMemery();
	void SetRecodeArraySize();
	int detect_peak(
        const double*   data, /* the data */ 
        int             data_count, /* row count of data */ 
        int*            emi_peaks, /* emission peaks will be put here */ 
        int*            num_emi_peaks, /* number of emission peaks found */
        int             max_emi_peaks, /* maximum number of emission peaks */ 
        int*            absop_peaks, /* absorption peaks will be put here */ 
        int*            num_absop_peaks, /* number of absorption peaks found */
        int             max_absop_peaks, /* maximum number of absorption peaks
                                            */ 
        double          delta, /* delta used for distinguishing peaks */
        int             emi_first /* should we search emission peak first of
                                     absorption peak first? */
        );
	void AMPThresholdIsOverfByPeakMethod(int Band_Max_hz,int size_);

	double* GetXArrayDisplay() { return X_Array_Display; }
	double* GetYArrayDisplay() { return Y_Array_Display; }
	double* GetYArrayDisplay2() { return Y_Array_Display_2; }
	//double* GetXArrayDisplay() { return X_Array_Display; }
	void SetOffsetOverThreshold(double _OffsetOverThreshold) { OffsetOverThresh = _OffsetOverThreshold; }

private:
	double AutoThreshOffset;
	int Recode_size_Capacity;
	bool Recoded_Data_Change_flag;//lock data array while change
	bool Recoded_Data_Change_flag2;

	bool IsRecoding_notrawdata;
	IIRProcDataStruct IirDataSet;

	double OffsetOverThresh;
	double Avg_amp_1sec;//just 1sec
	double Avg_amp_8sec;//just 8sec
	double Max_amp_8sec;//just 8sec
	double Avg_amp;//from start 
	double Total_amp;
	char name[20];
	UsbThread serialPort;
	FIRProcDataStruct FirDataSet;
	double org_Y;
	double width_;
	double scal_v;
	int Current_sample_indx;
	int CountOfRecodeSamples;

	bool IsRawWave;
	bool IsInhibit;//true inhibit false reward
	bool IsOverFlowed[2048];//data "amp" >thresh value ? true,false
	
	int CountofOverSample;
	double OverVpercent_8sec;//just 8sec
	double OverVpercent;//from start 
	double TargetOverPercent;

	double Y_Array_SM_WAVE[2048];
	double Y_Array_SM_WAVE_[2048];
	double X_Array_Display[2048];
	double Y_Array_Display[2048];//ch1
	double Y_Array_Display_2[2048];//ch2
	double Buf_1sec[256];//ch1	
	double Buf_1sec_2[256];//ch2	
	double Buf_2sec[512];//ch1	
	
	double FIR[2048];
	double ThreshValue;
	double RawThreshValueBuf;//half AMP value & SCALED 
	bool IsThreshVChange;
	int CountForThreshChange;
	double ThreshValue_buf;
	double Absolute_Thresh;
	vector<double>Recoded_Array;
	vector<bool>Recoded_Array_IsOver;
	bool recode_array_buf[2048];
	bool isHalfAutoProcessing;

	vector<double>Recoded_Array_2;
	vector<bool>Recoded_Array_IsOver_2;
	bool recode_array_buf_2[2048];
	bool isHalfAutoProcessing_2;

	int start_hz;
	int end_hz;

	bool FlagNeedNewThreshold;
	
	double amphz[15];

	double offset_Target;
	int Duration_for_auto;
	bool IsThresholdComputeEndding;

	double T_success;
	
	int T_SCount;
	bool IsLockedUSBGetting;

	double MinAMP;
	double MaxAMP;
	double SDAMP;//std dev

	double Total_AVG_AMP;
	vector<double>Avg_amp_8sec_Arry;
	void ComputeWaveAMPStdDev();

	bool IsSingleChanel;
	bool IsNeedComputeThreshold;

	vector<double> Peak_8sec_Arry;
	vector<int> Peak_8sec_IDX_Arry;
	
	IIRProc New_IIR_filter;
};

