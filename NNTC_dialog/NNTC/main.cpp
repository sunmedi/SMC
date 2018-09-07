#include "nntc.h"
#include "Registration.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//Registration w2;
	//w2.show();
	NNTC w;
	w.setGeometry(1920, 0, 1920, 1080);
	w.show();

	return a.exec();
}
