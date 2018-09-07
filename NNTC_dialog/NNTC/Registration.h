#ifndef Registration_H
#define Registration_H
#include "qdialog.h"
#include "ui_Registration.h"
#include <QtWidgets>
#include <QtSql>
//#include <qsqldatabase.h>

/**
*
* @class Registration
* @brief 사용자 추가 및 훈련 추가
* @details DB 로컬 서버에 훈련자 정보를 저장하고 관리한다. 
		   훈련자 정보는 신규 등록, 정보 수정, 정보 삭제, 검색이 있고
		   훈련 정보는 훈련 추가, 훈련 평가, 훈련 삭제, TLC 검사가 있다.
		   추가할 기능으로 뇌파 리뷰가 있다.
* @author 이보규
* @date 2018-09-05
* @version 0.0.1
*
*/
class Registration : public QDialog
{
	Q_OBJECT
public:
	explicit Registration(QWidget *parent = 0);
	~Registration();

	QSqlDatabase* SetDatabaseInfo(QString sql, QString ip, int port, QString dbname, QString usrname, QString password);
	bool databaseConnect();

	

private:
	void showError(const QSqlError & err);
	QSqlError initDb();
	void addUser(QSqlQuery & q, const QString & name, QDate birth, bool gender, int id, bool hand, QString com);


	Ui::Registration *ui;

	QSqlDatabase database;
	//QSqlRelationalTableModel *model;
};

#endif // Registration_H
