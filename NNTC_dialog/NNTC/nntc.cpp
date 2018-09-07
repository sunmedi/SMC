#include "nntc.h"
//#include "GLWidget.h"

NNTC::NNTC(QWidget *parent)
	: QDialog(parent, Qt::FramelessWindowHint),
	ui(new Ui::NNTCClass)
{
	ui->setupUi(this);

	processEXE = new QProcess(this);	
	socketUDPFromGame = new QUdpSocket(this);

	dataCtrl = new DataController(this);

	threshValueArray[0] = 60.0;//uv
	threshValueArray[1] = 10;//uv
	threshValueArray[2] = 5;//uv
	threshValueArray[3] = 10;//uv

	

	// OpenGL 위젯 추가
	//GLWidget *win = new GLWidget(this);
	//win->setGeometry(275, 61,1645, 1019);
//	grapgh = new GLWidget(this);
//	grapgh->setGeometry(275, 61, 1645, 1019);

	//grapgh->SetXArrayDisplay(dataCtrl->GetBuffer());

#pragma region UI 코드입력
	//// 라벨에 이미지 넣어서 추가 -> 배경화면
	//QLabel *toolbar = new QLabel(this);
	//toolbar->setGeometry(0, 0, 1920, 61);
	//toolbar->setStyleSheet("background-image: url(:/NNTC/Resources/page5/51.jpg);");
	//QLabel *leftbar = new QLabel(this);
	//leftbar->setGeometry(0, 61, 275, 1019);
	//leftbar->setStyleSheet("background-image: url(:/NNTC/Resources/page5/52.jpg);");
	//
	//// 버튼 추가

	//QPixmap mp("/NNTC/Resources/page5/play.png");
	//QPushButton *bnPlay = new QPushButton(this);
	//bnPlay->setGeometry(750, 10, 45, 45);
	////bnPlay->setStyleSheet("background-image: url(:/NNTC/Resources/page5/play.png);");
	//QBitmap TransparentMask(bnPlay->width(), bnPlay->height());
	//TransparentMask.fill(Qt::color1);
	//QPainter painter;
	//painter.begin(&TransparentMask);
	//painter.fillRect(QRect(0, 0, 45, 45), Qt::color0);
	//painter.end();
	//bnPlay->setMask(TransparentMask);
	//QPushButton *bnStop = new QPushButton(this);
	//QPushButton *bnPause = new QPushButton(this);
	//QPushButton *bnGame = new QPushButton(this);
	//
	//QPushButton *bnExit = new QPushButton(this);
#pragma endregion
	
	// 버튼 signal/slot 연결
	connect(ui->ButtonPlay, SIGNAL(clicked()), this, SLOT(OnPlayBtn()));
	connect(ui->ButtonStop, SIGNAL(clicked()), this, SLOT(OnStopBtn()));
	connect(ui->ButtonPause, SIGNAL(clicked()), this, SLOT(OnPauseBtn()));
	connect(ui->ButtonGame, SIGNAL(clicked()), this, SLOT(GameSelect()));
	connect(ui->ButtonExit, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));
	
	// 
//	socketUDPFromGame->bind(QHostAddress::LocalHost, 5150);
//	connect(socketUDPFromGame, SIGNAL(readyRead()), this, SLOT(readUdpData()));

	dataCtrl->InitDController(1920, 1080, threshValueArray);
}

NNTC::~NNTC()
{
	dataCtrl->stop();
	delete ui;
}


/**
*
*        @brief 플레이 버튼 실행
*        @details 플레이 버튼 눌렀을 때 실행하는 메서드이다.
*
*/
void NNTC::OnPlayBtn()
{
	if (!isPlayClicked)
	{
		dataCtrl->start();
		isPauseClicked = false;
		isPlayClicked = true;
	}
}	

/**
*
*        @brief 일시정지 버튼 실행
*        @details 일시정지 버튼 눌렀을 때 실행하는 메서드이다.
*
*/
void NNTC::OnPauseBtn()
{
	if (isPlayClicked)
	{
		// puase 버튼이 처음 눌렸을 때 isPauseClicked는 true가 된다.
		if (!isPauseClicked)
		{
			dataCtrl->pause();
			isPauseClicked = true;
		}
		// puase 버튼이 처음 눌렸을 때 isPauseClicked는 true가 된다.
		else if (isPauseClicked)
		{
			dataCtrl->resume();
			isPauseClicked = false;
		}
	}
}

/**
*
*        @brief 정지 버튼 실행
*        @details 정지 버튼 눌렀을 때 실행하는 메서드이다.
*
*/
void NNTC::OnStopBtn()
{
	dataCtrl->stop();
	isPauseClicked = false;
	isPlayClicked = false;
}

/**
*
*        @brief 게임 실행해보기 
*        @details 게임을 실행해보는 테스트 코드이다.
*
*/
void NNTC::GameSelect()
{
	// 실행 파일의 경로를 설정 한다.
	QString _exePath = "./game/game1.exe";
	// 실행 파일이 있는지 체크 한다.
	if (QFile::exists(_exePath))
	{
		// 실행 파일이 정상적으로 존재 한다면 QProcess 생성
		// 비교를 하는 이유는 다른 곳에서 생성했을 수도 있다.
		if (processEXE == NULL)
		{
			processEXE = new QProcess(this);
		}
		// 간혹 실행파일의 시작위치를 지정해 주지 않으면 오동작 하는 프로그램이 있다.
		//std::string _workingPath = (_exePath.toStdString()).substr(0, (_exePath.toStdString()).find_last_of('/') );
		//processEXE->setWorkingDirectory(QString::fromStdString(_workingPath));
		// 아규먼트가 필요하다면 이렇게 ..
		//QStringList _arguments;
		//_arguments << "1stArg" << "2ndArg";
		// 실행한다.
		//processEXE->start(_exePath, _arguments);
		// 아규먼트가 불필요 하다면 ...
		processEXE->start(_exePath);
	}
}

/**
*
*        @brief UDP 통신 보내기
*        @details UDP 통신을 통해서 뇌파 데이터를 게임으로 보낸다.
*        @throws ValidException 
*
*/
void NNTC::SendUdpData()
{
	QByteArray datasBuf;
	datasBuf.resize(130);
	for (int i = 0; i < 32; i++) {
		datasBuf[i] = 0;
	}
	for (int i = 32; i < 64; i++) {
		datasBuf[i] = 50;
	}
	for (int i = 64; i < 96; i++) {
		datasBuf[i] = 50;
	}
	for (int i = 96; i < 128; i++) {
		datasBuf[i] = 50;
	}
	datasBuf[129] = '1';
	socketUDPToGame->writeDatagram(datasBuf, QHostAddress::LocalHost, 5150);
}

/**
*
*        @brief UDP 통신 받기
*        @details UDP 통신을 통해서 게임의 상태를 받아온다.
*        @throws ValidException
*
*/
void NNTC::ReadUdpData()
{
	char *buff = new char[32];
	socketUDPFromGame->readDatagram(buff, 32);
	qDebug() << QString::fromUtf8("수신된 데이터 :") << buff;

}
