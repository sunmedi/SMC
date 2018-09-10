#pragma once
#include "qthread.h"
#include "qmutex.h"
#include <QWaitCondition>
#include <qdebug>
#include "UsbThread.h"

#define BUF_SIZE 32

/**
*
* @class UsbThread
* @brief �ø��� ��� ����
* @details �ø��� ����� �����带 �̿��Ͽ� ������ �޴´�. �׸��� �Ͻ������� �ٽ� ���۱���� �߰��Ͽ���.
* @author �̺���
* @warning ���� 17����Ʈ�� �о���� �۾��� �Ҿ����ϴ�.
* @date 2018-09-04
* @version 0.0.1
*
*/
class UsbDataThread : public QThread
{
public:
	UsbDataThread(QObject *parent = 0);
	~UsbDataThread();


	void SetBoolPause(bool flag) { isPause = flag; }	///< m_pause ������ �ʱ�ȭ �ϱ� ���� ����Ѵ�.
	void SetBoolStop(bool flag) { isStop = flag; }		///< m_stop ������ �ʱ�ȭ �ϱ� ���� ����Ѵ�.
	void SetCurrentDisplayIndex(int _currentDisplayIndex) { currentDisplayIndex = _currentDisplayIndex; }
	void Start() { isPause = false; isStop = false; data->InitThread(); data->start(); start(); }
	void Pause() { isPause = true; }	///< �����带 �Ͻ� �����Ѵ�.
	void Resume() { isPause = false; m_pauseCondition.wakeAll(); }	///< �Ͻ������� �����带 �ٽ� �����Ѵ�.
	void Stop() { isStop = true; }		///< �����带 �����Ѵ�.
	void SetChannelNumber(bool flag) { channelNumber = flag; }	///> ä�� ���� �����Ѵ�. false�� 1ä��, true�� 2ä��
	double* GetChannel1Data() { return buffer1; }		///< 1ä���� ����(Voltage) �����͸� ��ȯ�Ѵ�.
	double GetChannel2Data() { return buffer2; }		///< 2ä���� ����(Voltage) �����͸� ��ȯ�Ѵ�.

protected:
	bool ReadFromNeuronFlex(double *buffer, int packageSize, int endIndex);		///< 
	bool ReadFromNeuronFlex2(double *buffer1, double *buffer2, int packageSize, int endIndex);

private:
	void run() override;	///< �����尡 start()�� ȣ��Ǹ� �� �Լ��� ����ȴ�.

	QByteArray datas;		///< �ø��� ������� �޾ƿ� �����͸� �����Ѵ�. �� �����ʹ� GetByteArray()�� �ٸ� Ŭ������ ���� �� �ִ�.
	QMutex m_mutex;			///< �Ͻ������� ������ �����ش�.
	bool isStop;			///< true�� �����ϸ� ���� �����尡 �����Ѵ�.
	bool isPause;			///< true�� �����ϸ� ���� �����尡 �Ͻ������Ѵ�.
	QWaitCondition m_pauseCondition;	///< �Ͻ������� �� �� �ְ� ���ش�.
	bool m_quit = false;	///< run() �����带 ��� ���� ���� ������ ���Ѵ�. ������� �ʴ´�.

	bool channelNumber;		///< ä�� ���� �Է��ϴ� ��ȣ�̴�. false�� 1ä��,, true�� 2ä��
	double buffer1[2048], buffer2; ///< �ø��� �����͸� ����(Voltage)������ ��ȯ��Ų ���̴�.

	UsbThread *data;

	int currentDisplayIndex;
	double Buf_2sec[512];
	bool isOverFlow[2048];
	double dataChannel1Array[2048];
	double dataChannel2Array[2048];
};
