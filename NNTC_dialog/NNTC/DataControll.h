#pragma once
#include <qobject.h>
#include <qdebug.h>
#include <vector>
#include <qtimer.h>
#include "UsbThread.h"
//#include "EEGWave.h"

#define EXCELLENT_RAW 22

struct ThresholdChange
{
	int sampleInx;//In Recode;
	double thresholdV_RAW;
	double thresholdV_LI;
	double thresholdV_RE;
	double thresholdV_HI;
	double CurrentSuccessPerc_RAW;
	double CurrentSuccessPerc_LI;
	double CurrentSuccessPerc_RE;
	double CurrentSuccessPerc_HI;
};

/**
*
* @brief 데이터 통신을 조절한다.
* @details 시리얼 포트와 TCP/UDP 통신을 담당한다. 다이얼로그에서 버튼 이벤트를 실행했을 때	
*		   제어해준다. 통신을 통해 넘어온 데이터들을 관리 및 계산한다.
* @author 이보규
* @date 2018-09-04
* @version 0.0.1
*
*/
class DataController : public QObject
{
	Q_OBJECT
public:
	explicit DataController(QObject *parent);
	~DataController();
	
	void start();
	void pause();
	void resume();
	void stop();

	bool UpdateWave(double rawWave[], bool isRecording);

	void InitDController(int width_, int height_, double ThreshValue_A[]);
protected:

private slots :
	void update();

private:
	bool issingledata;
	bool IsFullAuto;//true full false half
	/// 데이터 세이브 파일 관련
	bool isautosaveOk;
	bool IsFirstSave;
	bool IsNeedToDeleteFile;
	int CountofDataInFile;
	int CountOfRawDataLines;
	int CountOfsavefile;
	/// 녹화시간 관련
	int RecodedTime;
	int ToltalTime;//played time
	int countofTotalReward;
	double PersofTotalReward;
	/// 콤보 관련
	int CountofComboo;
	double CombooPercent;
	/// 
	double OffsetOverThreshold;
	/// 
	bool Is_lock_fft_data;
	/// 
	bool isupdating;
	/// 
	unsigned char m_ColorID[22][3];
	///
	std::vector<ThresholdChange>ThresholdArry;
	bool isDataReady;
	/// 
	int width, height;
	/// 
	UsbThread usbSerialPort;
	///
	bool RawGreen[2048];
	///
	char Socket_Package_128[130];//128+2 “129”means isplaying '130'mean is update
	/// 
	int Raw_Excellent_Condition;
	/// Update 함수를 위한 타이머
	QTimer *timer;
	////////////////////////////
	std::vector<double>D_FFT;//DERTA AMP IN 1 SEC 0-4HZ
	std::vector<double>T_FFT;//THETA AMP IN 1 SEC 4-7
	std::vector<double>A_FFT;//ALPHA AMP IN 1 SEC 8-12
	std::vector<double>S_FFT;//SMR   AMP IN 1 SEC 12-15
	std::vector<double>L_FFT;//LBETA AMP IN 1 SEC 15-18
	std::vector<double>H_FFT;//HBETA AMP IN 1 SEC 22-36

	double SUM_D;
	double SUM_T;
	double SUM_A;
	double SUM_S;
	double SUM_L;
	double SUM_H;

	double MIN_D;
	double MIN_T;
	double MIN_A;
	double MIN_S;
	double MIN_L;
	double MIN_H;
	double MEAN_D;
	double MEAN_T;
	double MEAN_A;
	double MEAN_S;
	double MEAN_L;
	double MEAN_H;
	double MAX_D;
	double MAX_T;
	double MAX_A;
	double MAX_S;
	double MAX_L;
	double MAX_H;
	
	double SD_D;
	double SD_T;
	double SD_A;
	double SD_S;
	double SD_L;
	double SD_H;


};

