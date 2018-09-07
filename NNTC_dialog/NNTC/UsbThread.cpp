#include "UsbThread.h"
#include <qdebug.h>
#include <qtime>
#include <Windows.h>

UsbThread::UsbThread(QObject *parent)
	: QThread(parent)
{
	m_stop = false;
	m_pause = false;
	serial = new QSerialPort(this);

}


UsbThread::~UsbThread()
{
	serial->clear();
	serial->close();
	delete serial;
	m_mutex.lock();
	m_quit = true;
	m_mutex.unlock();
	wait();
}

/**
*
*        @brief USB 연결 쓰레드 시작
*        @details 시리얼 포트가 연결되어 있는지 판별해 준다.
*        @return boolean
*				 USB 연결에 성공하면 true, 실패하면 false를 반환한다.	
*        @throws ValidException 
*
*/
bool UsbThread::InitThread()
{
	return SerialConnect();
}

/**
*
*        @brief USB 연결 후 쓰레드 동작
*        @details 시리얼 포트가 연결 되어 있어야 데이터를 받아오는 쓰레드를 시작한다.
				  0.01초 동안 읽기 대기한 후에 17바이트를 읽어 온다. 17바이트를 넘는 데이터는 빈 버퍼로
				  넘겨서 읽지 않는다.
*        @throws ValidException
*
*/
void UsbThread::run()
{
	while (!m_quit) {
		if (m_stop)	break;
		m_mutex.lock();
		if (m_pause) m_pauseCondition.wait(&m_mutex, 100);

		if (channelNumber == false)ReadFromNeuronFlex(&buffer1);
		else if (channelNumber == true)ReadFromNeuronFlex2(&buffer1, &buffer2);
		
		m_mutex.unlock();
	}
}

/**
*
*        @brief 시리얼 포트 연결
*        @details 시리얼 포트 중에서 뉴론플렉스를 인식해서 연결한다. 
				  NeronFlex의 vendor Identifier와 Product Identifier는 각각 3871과 65529이다.
*        @return boolean
*				 USB 연결 성공시 true, 실패시 false 반환
*        @throws ValidException 현재 vendor와 product를 이용해서 장치의 고유 번호를 인식하는데
*								이 두 번호가 달리질 경우 연결이 되지 않는다. 그리고 2개의 뉴런플렉스를
*								연결했을 때의 처리가 되어 있지 않다.
*
*/
bool UsbThread::SerialConnect()
{
	// 시리얼 통신이 연결되었는지 확인한다.
	bool _isConnect = false;
	// 연결된 USB 통신을 모두 확인한다.
	foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
	{
		//QString _name = serialPortInfo.portName();
		//qDebug() << serialPortInfo.vendorIdentifier();
		//qDebug() << serialPortInfo.productIdentifier();
		// 현재 포트의 vendor와 product Identifier를 장치와 비교하여 지정한 장치면 
		if (serialPortInfo.vendorIdentifier() == 3871 && serialPortInfo.productIdentifier() == 65529)
		{
			// 현재 Port 번호를 저장한다.
			QString _name = serialPortInfo.portName();
			// 시리얼 포트에 현재 포트 번호를 지정하여 연결한다.
			serial->setPortName(_name);
			// 완료되면 true
			_isConnect = true;
		}
	}
	if (_isConnect)
	{
		serial->setBaudRate(QSerialPort::Baud115200);	///< Baudrate : 115200
		serial->setDataBits(QSerialPort::Data8);		///< DataBit : 8 bit
		serial->setParity(QSerialPort::NoParity);		///< ParityBit : no
		serial->setStopBits(QSerialPort::OneStop);		///< StopBit : 1 bot
		serial->setFlowControl(QSerialPort::NoFlowControl);		
		serial->open(QIODevice::ReadOnly);

		return true;
	}
	return false;
}


/**
*
*        @brief 데이터를 읽어서 계산
*        @details 시리얼 포트에서 받아온 데이터 중에서 17 바이트인 데이터만을 뇌파 데이터라고 인식한다.
voltage 값은 Raw data * 512/1023 - 256이다.
*        @param double buffer[] 아날로그 신호 값을 디지털로 변환하여 저장한다. 버퍼의 크기는 2048로 1초에 256개로 샘플링 된 데이터를
8초동안 저장할 공간이다.
*        @param int packageSize    256개의 샘플링 된 데이터 중에서 일정 영역을 선택해서 미리 처리하기 위한 사이즈를 지정한다.
*        @param int endIndx	   샘플링된 데이터의 현재 위치를 메소드에 알려준다.
*        @throws ValidException
*
*/
bool UsbThread::ReadFromNeuronFlex(double *buffer)
{
	if (serial->waitForReadyRead(4))
	{
		datas = serial->read(17);
		//qDebug() << (unsigned char)datas[0] << "  " << (unsigned char)datas[1] << "  " << (unsigned char)datas[2] << "  " << (unsigned char)datas[3] << "  " << (unsigned char)datas[4] << "  " << (unsigned char)datas[5] << "  " << (unsigned char)datas[6] << "  " << (unsigned char)datas[7] << "  " << (unsigned char)datas[8] << "  " << (unsigned char)datas[9];
	}
	if (datas.size() != 17) return false;
	if ((unsigned char)datas[0] == 165 && (unsigned char)datas[1] == 90)
	{
		//recvBuf_[i] = recvBuf[3];//count
		double _ch1 = datas[4] << 8 | datas[5];//AD transf
		*buffer = _ch1*512.0 / 1023.0 - 256.0;//EEG uV
		qDebug() << "usbThread : " << *buffer;

	}
	return true;
}

/**
*
*        @brief 2 채널의 데이터를 읽어서 계산
*        @details 시리얼 포트에서 받아온 데이터 중에서 17 바이트인 데이터만을 뇌파 데이터라고 인식한다.
voltage 값은 Raw data * 512/1023 - 256이다.
*        @param double buffer1[] 아날로그 신호 값을 디지털로 변환하여 저장한다. 버퍼의 크기는 2048로 1초에 256개로 샘플링 된 데이터를
								 8초동안 저장할 공간이다. 채널 1의 공간이다.
		 @param double buffer2[] 채널 2의 데이터를 저정할 공간이다.
*        @param int packageSize 256개의 샘플링 된 데이터 중에서 일정 영역을 선택해서 미리 처리하기 위한 사이즈를 지정한다.
*        @param int endIndx	샘플링된 데이터의 현재 위치를 메소드에 알려준다.
*        @throws ValidException
*
*/
bool UsbThread::ReadFromNeuronFlex2(double *buffer1, double *buffer2)
{
	if (serial->waitForReadyRead(10))
	{
		datas = serial->read(17);
		//qDebug() << (unsigned char)datas[0] << "  " << (unsigned char)datas[1] << "  " << (unsigned char)datas[2] << "  " << (unsigned char)datas[3] << "  " << (unsigned char)datas[4] << "  " << (unsigned char)datas[5] << "  " << (unsigned char)datas[6] << "  " << (unsigned char)datas[7] << "  " << (unsigned char)datas[8] << "  " << (unsigned char)datas[9];
	}
	if (datas.size() != 17) return false;
	if ((unsigned char)datas[0] == 165 && (unsigned char)datas[1] == 90)
	{
		double _ch1 = datas[4] * 256 + datas[5];//AD transf
		double _ch2 = datas[6] * 256 + datas[7];
		*buffer1 = (double)_ch1*512.0 / 1023.0 - 256.0;//EEG uV
		*buffer2 = (double)_ch2*512.0 / 1023.0 - 256.0;//EEG uV
	}

	return true;
}