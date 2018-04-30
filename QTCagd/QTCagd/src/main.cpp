#include "QTCagd.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTCagd w;
	w.show();
	return a.exec();
}
