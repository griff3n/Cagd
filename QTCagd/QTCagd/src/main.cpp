#include "QTCagd.h"
#include <QtWidgets/QApplication>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTCagd w;
	w.show();
	int r = a.exec();
	_CrtDumpMemoryLeaks();
	return r;
}
