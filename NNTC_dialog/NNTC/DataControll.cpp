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
	usbSerialPort.exit();
}

/**
*
*        @brief usb ��� ����, �Ͻ�����, �ٽ� ����, ���� ����
*        @details Start() �Լ��� �����ϸ� USB�� �����ϰ� �����带 �����Ѵ�. 
				  Pause() �Լ��� �����带 �Ͻ� ���� ��Ų��.
				  Resume()�Լ��� �����带 �ٽ� �����Ѵ�.
				  Stop()�Լ��� �����带 �����Ѵ�.
*        @throws ValidException
*
*/
void DataController::start()
{
	usbSerialPort.InitThread();
	usbSerialPort.Start();
}
void DataController::pause()
{
	usbSerialPort.Pause();
}
void DataController::resume()
{
	usbSerialPort.Resume();

}
void DataController::stop()
{
	usbSerialPort.Stop();
}

/**
*
*        @brief ���� ���̺� ������Ʈ
*        @details ������ �׸��� ������ ���� �����͸� ������Ʈ�Ѵ�.
*        @throws ValidException
*
*/
bool DataController::UpdateWave(double rawWave[], bool isRecording)
{
	return true;
}

void DataController::InitDController(int width_, int height_, double ThreshValue_A[])
{
	
}

void DataController::update()
{

}
