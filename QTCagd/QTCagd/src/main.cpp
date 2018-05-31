#include "QTCagd.h"
#include <QtWidgets/QApplication>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(int argc, char *argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	QApplication a(argc, argv);
	QTCagd w;
	w.show();
	return a.exec();
}
