#ifndef NNTC_H
#define NNTC_H

#include <QtWidgets/QDialog>
#include "ui_nntc.h"
#include <qdebug.h>
#include <qfile.h>
#include <qprocess.h>
#include <QUdpSocket>
#include <Qthread>
#include <DataControll.h>


/**
*
* @class NNTC
* @brief EEG-Training 다이얼로그
* @details 시리얼 포트로 뉴런플렉스와 연결하고 뇌파 값을 읽어와서 파형을 그려준다. 게임을
*		   실행하여 뇌파 값을 TCP 통신으로 게임에 보내주고 게임의 현재 상태를 UDP 통신으로
*		   메인 프로그램에서 받는다.
* @author 이보규
* @date 2018-09-03
* @version 0.0.1
*
*/
class NNTC : public QDialog
{
	Q_OBJECT

public:
	explicit NNTC(QWidget *parent = 0);
	~NNTC();

protected:

public slots :
	void OnPlayBtn();
	void OnStopBtn();
	void OnPauseBtn();
	void GameSelect();
	void SendUdpData();
	void ReadUdpData();

private:
	Ui::NNTCClass *ui;
	QProcess *processEXE;			///< 외부 파일 실행을 위한 QProcess
	QUdpSocket *socketUDPFromGame;	///< 
	QUdpSocket *socketUDPToGame;	///< 

	DataController *dataCtrl;

	double threshValueArray[4];//a chanal's thresh value 
							//index0 means raw data's amp thresh
							//index1 means L-inhibite data's amp thresh
							//index2 means Reward     data's amp thresh
							//index3 means H-inhibite data's amp thresh

	QThread usbDataThread;
//	GLWidget *grapgh;
	/** 
	*	버튼 클릭 여부
	*/
	bool isPauseClicked = false;
	bool isPlayClicked = false;

};

#endif // NNTC_H
