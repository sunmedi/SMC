#pragma once
#include "qthread.h"
#include "qserialport.h"
#include "qserialportinfo.h"
#include "qmutex.h"
#include <QWaitCondition>


/**
*
* @class UsbThread
* @brief 시리얼 통신 제어
* @details 시리얼 통신을 쓰레드를 이용하여 보내고 받는다. 그리고 일시정지와 다시 시작기능을 추가하였다.
* @author 이보규
* @warning 아직 17바이트를 읽어오는 작업이 불안정하다.
* @date 2018-09-04
* @version 0.0.1
*
*/
class UsbThread :
	public QThread
{
	Q_OBJECT
public:
	UsbThread(QObject *parent = 0);
	~UsbThread();

	bool InitThread();		///< 쓰레드를 초기화 해준다. serial_connect()함수를 불러온다.

	//QByteArray GetByteArray() { return datas; }			///< 시리얼 통신을 통해 읽어온 데이터를 반환해 준다.
	void SetBoolPause(bool flag) { m_pause = flag; }	///< m_pause 변수를 초기화 하기 위해 사용한다.
	void SetBoolStop(bool flag) { m_stop = flag; }		///< m_stop 변수를 초기화 하기 위해 사용한다.
	void SetChannelNumber(bool flag) { channelNumber = flag; }	///> 채널 수를 지정한다. false는 1채널, true는 2채널
	double GetChannel1Data() { return buffer1; }		///< 1채널의 전위(Voltage) 데이터를 반환한다.
	double GetChannel2Data() { return buffer2; }		///< 2채널의 전위(Voltage) 데이터를 반환한다.
	void Pause() { m_pause = true; }	///< 쓰레드를 일시 정지한다.
	void Resume() { m_pause = false; m_pauseCondition.wakeAll(); }	///< 일시정지한 쓰레드를 다시 시작한다.
	void Stop() { m_stop = true; }		///< 쓰레드를 정지한다.

protected:
	bool SerialConnect();		///< 시리얼포트를 연결하기 위한 초기 설정을 한다.
	bool ReadFromNeuronFlex(double *buffer);		///< 
	bool ReadFromNeuronFlex2(double *buffer1, double *buffer2);

signals:
	void Timeout(const QString &s);	///< 입력대기가 일정시간 지나면 호출된다.

private:
	void run() override;	///< 쓰레드가 start()가 호출되면 이 함수가 실행된다.

	QByteArray datas;		///< 시리얼 통신으로 받아온 데이터를 저장한다. 이 데이터는 GetByteArray()로 다른 클래스로 보낼 수 있다.
	QSerialPort  *serial;	///< 시리얼 통신을 열기 위한 변수
	QMutex m_mutex;			///< 일시정지할 구간을 정해준다.
	bool m_stop;			///< true로 설정하면 현재 쓰레드가 정지한다.
	bool m_pause;			///< true로 설정하면 현재 쓰레드가 일시정지한다.
	QWaitCondition m_pauseCondition;	///< 일시정지를 할 수 있게 해준다.
	bool m_quit = false;	///< run() 쓰레드를 계속 돌지 빠져 나올지 정한다. 사용하지 않는다.

	bool channelNumber;		///< 채널 수를 입력하는 신호이다. false는 1채널,, true는 2채널
	double buffer1, buffer2; ///< 시리얼 데이터를 전위(Voltage)값으로 변환시킨 값이다.
};

