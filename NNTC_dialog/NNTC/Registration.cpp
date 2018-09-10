
#include "Registration.h"
#include <TableUserDelegate.h>
#include  <QtSql>

Registration::Registration(QWidget *parent)
	: QDialog(parent, Qt::FramelessWindowHint),
	ui(new Ui::Registration)
{
	ui->setupUi(this);

	database = QSqlDatabase::addDatabase("QMYSQL");
	database.setHostName("localhost");
	database.setDatabaseName("SMCinfo");
	database.setUserName("root");
	database.setPassword("sun462800");
	if (!database.open())
	{
		QMessageBox::critical(0, QObject::tr("Database Error"),
			database.lastError().text());
	}

	QSqlQuery query("SELECT * FROM test");

	QSqlQueryModel *model = new QSqlQueryModel();
	model->setHeaderData(0, Qt::Horizontal, tr("name"));
	model->setHeaderData(1, Qt::Horizontal, tr("birth"));
	model->setHeaderData(2, Qt::Horizontal, tr("gender"));
	model->setHeaderData(3, Qt::Horizontal, tr("id"));
	model->setHeaderData(4, Qt::Horizontal, tr("hand"));

	ui->TableUserInfo->setModel(model);
	
	
	
	//if (!QSqlDatabase::drivers().contains("QSQLITE"))
	//	QMessageBox::critical(
	//		this,
	//		"Unable to load database",
	//		"This demo needs the SQLITE driver"
	//	);
	//// Initialize the database:
	//QSqlError err = initDb();
	//if (err.type() != QSqlError::NoError) {
	//	showError(err);
	//	return;
	//}
	//// Create the data model:
	//model = new QSqlRelationalTableModel(ui->TableUserInfo);
	//model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	//model->setTable("UserInfo");
	//// Set the localized header captions:
	//model->setHeaderData(model->fieldIndex("name"),	Qt::Horizontal, tr("name"));
	//model->setHeaderData(model->fieldIndex("birth"), Qt::Horizontal, tr("birth"));
	//model->setHeaderData(model->fieldIndex("gender"), Qt::Horizontal, tr("gender"));
	//model->setHeaderData(model->fieldIndex("id"), Qt::Horizontal, tr("id"));
	//model->setHeaderData(model->fieldIndex("hand"), Qt::Horizontal, tr("hand"));
	//// Populate the model:
	//if (!model->select()) {
	//	showError(model->lastError());
	//	return;
	//}
	//// Set the model and hide the ID column:
	//ui->TableUserInfo->setModel(model);
	//ui->TableUserInfo->setItemDelegate(new TableUserDelegate(ui->TableUserInfo));
	//ui->TableUserInfo->setColumnHidden(model->fieldIndex("id"), true);
	//ui->TableUserInfo->setSelectionMode(QAbstractItemView::SingleSelection);
}


Registration::~Registration()
{
}

QSqlDatabase * Registration::SetDatabaseInfo(QString sql, QString ip, int port, QString dbname, QString usrname, QString password)
{
	return nullptr;
}

bool Registration::databaseConnect()
{
	return false;
}

QSqlError Registration::initDb()
{
	database = QSqlDatabase::addDatabase("QMYSQL");
	database.setHostName("127.0.0.1");
	database.setPort(3301);
	database.setDatabaseName("SMCinfo");
	database.setUserName("root");
	database.setPassword("sun462800");

	//bool ok = database.open();
/*
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(":SMCinfo:");*/

	if (!database.open())
		return database.lastError();

	QStringList tables = database.tables();
	if (tables.contains("UserInfo", Qt::CaseInsensitive))
		return QSqlError();

	QSqlQuery q;
//	if (!q.exec(QLatin1String("create table books(id integer primary key, name, varchar, birth date, gender boolean, id integer, hand boolean, connectCom varchar)")))
//		return q.lastError();

	if (!q.prepare(QLatin1String("insert into UserInfo(name, birth, gender, id, hand, connectCom) values(?, ?, ?, ?, ?, ?)")))
		return q.lastError();
	addUser(q, QLatin1String("이보규"), QDate(1948, 4, 28), false, 11111, false, QLatin1String("com01"));
	

	return QSqlError();
}

void Registration::addUser(QSqlQuery &q, const QString &name, QDate birth, bool gender, int id, bool hand, QString com)
{
	q.addBindValue(name);
	q.addBindValue(birth);
	q.addBindValue(gender);
	q.addBindValue(id);
	q.addBindValue(hand);
	q.addBindValue(com);
	q.exec();
}

void Registration::showError(const QSqlError &err)
{
	QMessageBox::critical(this, "Unable to initialize Database",
		"Error initializing database: " + err.text());
}
