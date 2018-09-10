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
* @brief ����� �߰� �� �Ʒ� �߰�
* @details DB ���� ������ �Ʒ��� ������ �����ϰ� �����Ѵ�. 
		   �Ʒ��� ������ �ű� ���, ���� ����, ���� ����, �˻��� �ְ�
		   �Ʒ� ������ �Ʒ� �߰�, �Ʒ� ��, �Ʒ� ����, TLC �˻簡 �ִ�.
		   �߰��� ������� ���� ���䰡 �ִ�.
* @author �̺���
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
