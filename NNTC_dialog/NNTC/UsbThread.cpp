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
*        @brief USB ���� ������ ����
*        @details �ø��� ��Ʈ�� ����Ǿ� �ִ��� �Ǻ��� �ش�.
*        @return boolean
*				 USB ���ῡ �����ϸ� true, �����ϸ� false�� ��ȯ�Ѵ�.	
*        @throws ValidException 
*
*/
bool UsbThread::InitThread()
{
	return SerialConnect();
}

/**
*
*        @brief USB ���� �� ������ ����
*        @details �ø��� ��Ʈ�� ���� �Ǿ� �־�� �����͸� �޾ƿ��� �����带 �����Ѵ�.
				  0.01�� ���� �б� ����� �Ŀ� 17����Ʈ�� �о� �´�. 17����Ʈ�� �Ѵ� �����ʹ� �� ���۷�
				  �Ѱܼ� ���� �ʴ´�.
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
*        @brief �ø��� ��Ʈ ����
*        @details �ø��� ��Ʈ �߿��� �����÷����� �ν��ؼ� �����Ѵ�. 
				  NeronFlex�� vendor Identifier�� Product Identifier�� ���� 3871�� 65529�̴�.
*        @return boolean
*				 USB ���� ������ true, ���н� false ��ȯ
*        @throws ValidException ���� vendor�� product�� �̿��ؼ� ��ġ�� ���� ��ȣ�� �ν��ϴµ�
*								�� �� ��ȣ�� �޸��� ��� ������ ���� �ʴ´�. �׸��� 2���� �����÷�����
*								�������� ���� ó���� �Ǿ� ���� �ʴ�.
*
*/
bool UsbThread::SerialConnect()
{
	// �ø��� ����� ����Ǿ����� Ȯ���Ѵ�.
	bool _isConnect = false;
	// ����� USB ����� ��� Ȯ���Ѵ�.
	foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
	{
		//QString _name = serialPortInfo.portName();
		//qDebug() << serialPortInfo.vendorIdentifier();
		//qDebug() << serialPortInfo.productIdentifier();
		// ���� ��Ʈ�� vendor�� product Identifier�� ��ġ�� ���Ͽ� ������ ��ġ�� 
		if (serialPortInfo.vendorIdentifier() == 3871 && serialPortInfo.productIdentifier() == 65529)
		{
			// ���� Port ��ȣ�� �����Ѵ�.
			QString _name = serialPortInfo.portName();
			// �ø��� ��Ʈ�� ���� ��Ʈ ��ȣ�� �����Ͽ� �����Ѵ�.
			serial->setPortName(_name);
			// �Ϸ�Ǹ� true
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
*        @brief �����͸� �о ���
*        @details �ø��� ��Ʈ���� �޾ƿ� ������ �߿��� 17 ����Ʈ�� �����͸��� ���� �����Ͷ�� �ν��Ѵ�.
voltage ���� Raw data * 512/1023 - 256�̴�.
*        @param double buffer[] �Ƴ��α� ��ȣ ���� �����з� ��ȯ�Ͽ� �����Ѵ�. ������ ũ��� 2048�� 1�ʿ� 256���� ���ø� �� �����͸�
8�ʵ��� ������ �����̴�.
*        @param int packageSize    256���� ���ø� �� ������ �߿��� ���� ������ �����ؼ� �̸� ó���ϱ� ���� ����� �����Ѵ�.
*        @param int endIndx	   ���ø��� �������� ���� ��ġ�� �޼ҵ忡 �˷��ش�.
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
*        @brief 2 ä���� �����͸� �о ���
*        @details �ø��� ��Ʈ���� �޾ƿ� ������ �߿��� 17 ����Ʈ�� �����͸��� ���� �����Ͷ�� �ν��Ѵ�.
voltage ���� Raw data * 512/1023 - 256�̴�.
*        @param double buffer1[] �Ƴ��α� ��ȣ ���� �����з� ��ȯ�Ͽ� �����Ѵ�. ������ ũ��� 2048�� 1�ʿ� 256���� ���ø� �� �����͸�
								 8�ʵ��� ������ �����̴�. ä�� 1�� �����̴�.
		 @param double buffer2[] ä�� 2�� �����͸� ������ �����̴�.
*        @param int packageSize 256���� ���ø� �� ������ �߿��� ���� ������ �����ؼ� �̸� ó���ϱ� ���� ����� �����Ѵ�.
*        @param int endIndx	���ø��� �������� ���� ��ġ�� �޼ҵ忡 �˷��ش�.
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