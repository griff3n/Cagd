#include "QTCadg.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTCadg w;
	w.show();
	return a.exec();
}
