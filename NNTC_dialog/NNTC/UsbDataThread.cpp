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
*        @brief USB ���� �� ������ ����
*        @details �ø��� ��Ʈ�� ���� �Ǿ� �־�� �����͸� �޾ƿ��� �����带 �����Ѵ�.
0.01�� ���� �б� ����� �Ŀ� 17����Ʈ�� �о� �´�. 17����Ʈ�� �Ѵ� �����ʹ� �� ���۷�
�Ѱܼ� ���� �ʴ´�.
*        @throws ValidException
*
*/
void UsbDataThread::run()
{
	while (!m_quit) {
		if (isStop)	break;
		m_mutex.lock();
		if (isPause) m_pauseCondition.wait(&m_mutex, 100);
		/// ������ ����

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


			int csi = currentDisplayIndex + BUF_SIZE;//������ �ۿ��� indx�����ϰ��ֱ⶧��.���� ...����...

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
