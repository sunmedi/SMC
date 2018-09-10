#include "UsbDataThread.h"



UsbDataThread::UsbDataThread(QObject *parent)
	: QThread(parent)
{
	data = new UsbThread(this);
	
}


UsbDataThread::~UsbDataThread()
{
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
void UsbDataThread::run()
{
	while (!m_quit) {
		if (isStop)	break;
		m_mutex.lock();
		if (isPause) m_pauseCondition.wait(&m_mutex, 100);
		/// 실행할 내용

		if (currentDisplayIndex <= 2048)
		{
			if (currentDisplayIndex == 2048)
			{
				for (int i = 1536, k = 0; i < 2048; k++, i++)
				{
					Buf_2sec[k] = dataChannel1Array[i];
				}
				for (int i = 0; i< 2048; i++)
				{
					isOverFlow[i] = false;//data "amp" >thresh value ? true,false;
					dataChannel1Array[i] = 0;
				}
				currentDisplayIndex = 0;
			}


			int csi = currentDisplayIndex + BUF_SIZE;//스레드 밖에도 indx공유하고있기때문.머저 ...다음...

			if (channelNumber == false)ReadFromNeuronFlex(buffer1, BUF_SIZE, csi);
			else if (channelNumber == true)ReadFromNeuronFlex2(buffer1, &buffer2, BUF_SIZE, csi);
			//dataChannel1Array[i] = buffer1[i];
			//qDebug() << "DataArray : " << dataChannel1Array[i];

			currentDisplayIndex += BUF_SIZE;

			int kk = 0;

			//just for raw data
			bool isover = false;
			//Y_Array_Display[i] *= 1.8;
			if (abs(dataChannel1Array[currentDisplayIndex]) > 0.5 * 60)
			{
				isOverFlow[currentDisplayIndex] = true;
				isover = true;
			}
		}

		m_mutex.unlock();
	}
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
bool UsbDataThread::ReadFromNeuronFlex(double *buffer, int packageSize, int endIndex)
{
	for (int i = endIndex - packageSize; i < endIndex; i++)
	{
//		datas = data->GetByteArray();
		if (datas.size() != 17) return false;
		if ((unsigned char)datas[0] == 165 && (unsigned char)datas[1] == 90)
		{
			//recvBuf_[i] = recvBuf[3];//count
			double _ch1 = datas[4] << 8 | datas[5];//AD transf
			buffer[i] = _ch1*512.0 / 1023.0 - 256.0;//EEG uV
			qDebug() << "Buffer : " << buffer[i];
		}
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
bool UsbDataThread::ReadFromNeuronFlex2(double *buffer1, double *buffer2, int packageSize, int endIndex)
{
	for (int i = endIndex - packageSize; i < endIndex; i++)
	{

		if (datas.size() != 17) return false;
		if ((unsigned char)datas[0] == 165 && (unsigned char)datas[1] == 90)
		{
			double _ch1 = datas[4] * 256 + datas[5];//AD transf
			double _ch2 = datas[6] * 256 + datas[7];
			buffer1[i] = (double)_ch1*512.0 / 1023.0 - 256.0;//EEG uV
			*buffer2 = (double)_ch2*512.0 / 1023.0 - 256.0;//EEG uV
		}
	}
	return true;
}

void UsbThread::update()
{
	//qDebug() << serial->bytesAvailable();
	//qDebug() << (unsigned char)datas[0] << "  " << (unsigned char)datas[1] << "  " << (unsigned char)datas[2] << "  " << (unsigned char)datas[3] << "  " << (unsigned char)datas[4] << "  " << (unsigned char)datas[5] << "  " << (unsigned char)datas[6] << "  " << (unsigned char)datas[7] << "  " << (unsigned char)datas[8] << "  " << (unsigned char)datas[9];

}
