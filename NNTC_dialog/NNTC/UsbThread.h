#pragma once
#include "qthread.h"
#include "qserialport.h"
#include "qserialportinfo.h"
#include "qmutex.h"
#include <QWaitCondition>


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
class UsbThread :
	public QThread
{
	Q_OBJECT
public:
	UsbThread(QObject *parent = 0);
	~UsbThread();

	bool InitThread();		///< �����带 �ʱ�ȭ ���ش�. serial_connect()�Լ��� �ҷ��´�.

	//QByteArray GetByteArray() { return datas; }			///< �ø��� ����� ���� �о�� �����͸� ��ȯ�� �ش�.
	void SetBoolPause(bool flag) { m_pause = flag; }	///< m_pause ������ �ʱ�ȭ �ϱ� ���� ����Ѵ�.
	void SetBoolStop(bool flag) { m_stop = flag; }		///< m_stop ������ �ʱ�ȭ �ϱ� ���� ����Ѵ�.
	void SetChannelNumber(bool flag) { channelNumber = flag; }	///> ä�� ���� �����Ѵ�. false�� 1ä��, true�� 2ä��
	double GetChannel1Data() { return buffer1; }		///< 1ä���� ����(Voltage) �����͸� ��ȯ�Ѵ�.
	double GetChannel2Data() { return buffer2; }		///< 2ä���� ����(Voltage) �����͸� ��ȯ�Ѵ�.
	void Pause() { m_pause = true; }	///< �����带 �Ͻ� �����Ѵ�.
	void Resume() { m_pause = false; m_pauseCondition.wakeAll(); }	///< �Ͻ������� �����带 �ٽ� �����Ѵ�.
	void Stop() { m_stop = true; }		///< �����带 �����Ѵ�.

protected:
	bool SerialConnect();		///< �ø�����Ʈ�� �����ϱ� ���� �ʱ� ������ �Ѵ�.
	bool ReadFromNeuronFlex(double *buffer);		///< 
	bool ReadFromNeuronFlex2(double *buffer1, double *buffer2);

signals:
	void Timeout(const QString &s);	///< �Է´�Ⱑ �����ð� ������ ȣ��ȴ�.

private:
	void run() override;	///< �����尡 start()�� ȣ��Ǹ� �� �Լ��� ����ȴ�.

	QByteArray datas;		///< �ø��� ������� �޾ƿ� �����͸� �����Ѵ�. �� �����ʹ� GetByteArray()�� �ٸ� Ŭ������ ���� �� �ִ�.
	QSerialPort  *serial;	///< �ø��� ����� ���� ���� ����
	QMutex m_mutex;			///< �Ͻ������� ������ �����ش�.
	bool m_stop;			///< true�� �����ϸ� ���� �����尡 �����Ѵ�.
	bool m_pause;			///< true�� �����ϸ� ���� �����尡 �Ͻ������Ѵ�.
	QWaitCondition m_pauseCondition;	///< �Ͻ������� �� �� �ְ� ���ش�.
	bool m_quit = false;	///< run() �����带 ��� ���� ���� ������ ���Ѵ�. ������� �ʴ´�.

	bool channelNumber;		///< ä�� ���� �Է��ϴ� ��ȣ�̴�. false�� 1ä��,, true�� 2ä��
	double buffer1, buffer2; ///< �ø��� �����͸� ����(Voltage)������ ��ȯ��Ų ���̴�.
};

