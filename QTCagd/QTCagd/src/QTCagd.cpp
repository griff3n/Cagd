#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include "QTCagd.h"


QTCagd::QTCagd(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void QTCagd::closeApplication()
{
	return;
}

void QTCagd::showBunny()
{
	/*auto filename = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Wavefront OBJ (*.obj)"));

	if (filename.isEmpty())
		return;

	auto future = QtConcurrent::run(viscas::ObjectLoader<viscas::half_edge_mesh>::loadOBJreg, filename.toStdString());

	ui->actionOpen->setEnabled(false);
	emit ui->statusBar->showMessage("Parsing file...");
	parseWatcher.setFuture(future);*/
}