#include "DataControll.h"

DataController::DataController(QObject *parent)
	: QObject(parent)
{
	issingledata = true;
	IsNeedToDeleteFile = false;
	SUM_D = 0.0;
	SUM_T = 0.0;
	SUM_A = 0.0;
	SUM_S = 0.0;
	SUM_L = 0.0;
	SUM_H = 0.0;

	SD_D = 0.0;
	SD_T = 0.0;
	SD_A = 0.0;
	SD_S = 0.0;
	SD_L = 0.0;
	SD_H = 0.0;

	MAX_D = 0.0;
	MAX_T = 0.0;
	MAX_A = 0.0;
	MAX_S = 0.0;
	MAX_L = 0.0;
	MAX_H = 0.0;
	MEAN_D = 0.0;
	MEAN_T = 0.0;
	MEAN_A = 0.0;
	MEAN_S = 0.0;
	MEAN_L = 0.0;
	MEAN_H = 0.0;
	MIN_D = 10000.0;
	MIN_T = 10000.0;
	MIN_A = 10000.0;
	MIN_S = 10000.0;
	MIN_L = 10000.0;
	MIN_H = 10000.0;

	CountofDataInFile = 0;

	IsFullAuto = true;
	CountOfRawDataLines = 0;
	isautosaveOk = true;
	CountOfsavefile = 0;
	IsFirstSave = true;
	PersofTotalReward = countofTotalReward = RecodedTime = 0;
	CombooPercent = 0.0;
	CountofComboo = 0;
	OffsetOverThreshold = 0.1;
	Is_lock_fft_data = false;
	
	isupdating = false;
	for (int i = 0; i < 22; i++)
	{
		if (i <= 10)
		{
			m_ColorID[i][0] = 0;
			m_ColorID[i][1] = 255 - i;
			m_ColorID[i][2] = 0;
		}
		else
		{
			m_ColorID[i][0] = 255 - i + 11;
			m_ColorID[i][1] = 0;
			m_ColorID[i][2] = 0;
		}
	}

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this, SLOT(update()));
}


DataController::~DataController()
{
	WaveArray[0].CloseUsbComm();
}

void DataController::start()
{
	WaveArray[0].OpenUsbComm();
	WaveArray[0].start();
	timer->start(4);
}

void DataController::pause()
{
	WaveArray[0].pause();
	//timer->nsleep(1000);
}

void DataController::resume()
{
	WaveArray[0].resume();
	//timer->start(1);

}

void DataController::stop()
{
	WaveArray[0].stop();
	timer->stop();
}

/**
*
*        @brief 위젯 웨이브 업데이트
*        @details 위젯에 그리는 파형에 대한 데이터를 업데이트한다.
*        @throws ValidException
*
*/
bool DataController::UpdateWave(double rawWave[], bool isRecording)
{
	return true;
}

void DataController::InitDController(int width_, int height_, double ThreshValue_A[])
{
	width = width_;
	height = height_;
	IsFirstSave = true;
	IsNeedToDeleteFile = true;
	SUM_D = 0.0;
	SUM_T = 0.0;
	SUM_A = 0.0;
	SUM_S = 0.0;
	SUM_L = 0.0;
	SUM_H = 0.0;

	MIN_D = 10000.0;
	MIN_T = 10000.0;
	MIN_A = 10000.0;
	MIN_S = 10000.0;
	MIN_L = 10000.0;
	MIN_H = 10000.0;

	MAX_D = 0.0;
	MAX_T = 0.0;
	MAX_A = 0.0;
	MAX_S = 0.0;
	MAX_L = 0.0;
	MAX_H = 0.0;

	MEAN_D = 0.0;
	MEAN_T = 0.0;
	MEAN_A = 0.0;
	MEAN_S = 0.0;
	MEAN_L = 0.0;
	MEAN_H = 0.0;

	SD_D = 0.0;
	SD_T = 0.0;
	SD_A = 0.0;
	SD_S = 0.0;
	SD_L = 0.0;
	SD_H = 0.0;

	SD_D = 0.0;
	SD_T = 0.0;
	SD_A = 0.0;
	SD_S = 0.0;
	SD_L = 0.0;
	SD_H = 0.0;
	if (ThresholdArry.empty())
	{
		ThresholdChange temp;
		temp.CurrentSuccessPerc_RAW = 0;
		temp.CurrentSuccessPerc_HI = 0;
		temp.CurrentSuccessPerc_LI = 0;
		temp.CurrentSuccessPerc_RE = 0;
		temp.sampleInx = 0;
		temp.thresholdV_RAW = ThreshValue_A[0];
		temp.thresholdV_LI = ThreshValue_A[1];
		temp.thresholdV_RE = ThreshValue_A[2];
		temp.thresholdV_HI = ThreshValue_A[3];
		ThresholdArry.push_back(temp);
	}
	CountofDataInFile = 0;


	isDataReady = false;


	IIRProcDataStruct IirDataSet;
	IirDataSet.PassType = 3;
	IirDataSet.IIRType = 4;
	IirDataSet.OmegaC = 50 / 128.0;
	IirDataSet.BandWidth = 100.0 / 128.0;

	char name[20] = "Raw";
	char name1[20] = "Low Inhibit";
	char name2[20] = "Reward";
	char name3[20] = "High Inhibit";

	int w_;

	if (!issingledata)
		w_ = 0.5*width;
	else
		w_ = width;
	width = w_;
	
	WaveArray[0].InitValue(issingledata, name, ThreshValue_A[0], w_, height*0.675, true, IirDataSet, true, 0.3, 50.0, 256, 5, 4, 15);


	IirDataSet.OmegaC = 5.5 / 128.0;
	IirDataSet.BandWidth = 3.0 / 128.0;
	WaveArray[1].InitValue(issingledata, name1, ThreshValue_A[1], w_, height*0.48, false, IirDataSet, true, 6.0, 80.0, 256, 5, 4, 15);

	IirDataSet.OmegaC = 13.5 / 128.0;
	IirDataSet.BandWidth = 3.0 / 128.0;
	WaveArray[2].InitValue(issingledata, name2, ThreshValue_A[2], w_, height*0.29, false, IirDataSet, false, 7.0, 65.0, 256, 5, 4, 15);

	IirDataSet.OmegaC = 29.0 / 128.0;
	IirDataSet.BandWidth = 14.0 / 128.0;
	WaveArray[3].InitValue(issingledata, name3, ThreshValue_A[3], w_, height*0.1, false, IirDataSet, true, 5.0, 85.0, 256, 5, 4, 15);

	WaveArray[0].SetOffsetOverThreshold(OffsetOverThreshold);
	WaveArray[1].SetOffsetOverThreshold(OffsetOverThreshold);
	WaveArray[2].SetOffsetOverThreshold(OffsetOverThreshold);
	WaveArray[3].SetOffsetOverThreshold(OffsetOverThreshold);

	for (int i = 0; i< 130; i++)
	{
		Socket_Package_128[i] = '0';
	}
	for (int i = 0; i< 2048; i++)
	{
		RawGreen[i] = false;
	}

	Raw_Excellent_Condition = EXCELLENT_RAW;
}

void DataController::update()
{
	WaveArray[0].GetRawDataFromUSB(false);
}
