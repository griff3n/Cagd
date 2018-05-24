#include "QTCagd.h"


QTCagd::QTCagd(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(&parseWatcher, SIGNAL(finished()), this, SLOT(parsingDone()));
}

void QTCagd::closeApplication()
{
	return;
}

void QTCagd::openFile()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Wavefront OBJ (*.obj)"));

	if (filename.isEmpty())
		return;

	HalfEdgeMesh* newMesh = new HalfEdgeMesh();
	QFuture<HalfEdgeMesh *> future = QtConcurrent::run(loadOBJreg, filename.toStdString());

	ui.action_Open->setEnabled(false);
	emit ui.statusBar->showMessage("Datei wird eingelesen...");
	parseWatcher.setFuture(future);
}

void QTCagd::parsingDone()
{
	QFuture<HalfEdgeMesh *> future = parseWatcher.future();
	HalfEdgeMesh* newHalfEdgeMesh = future.result();

	if (newHalfEdgeMesh)
	{
		emit meshChanged(newHalfEdgeMesh);
		emit ui.statusBar->showMessage(QLatin1String("Datei erfolgreich geöffnet!"), 5000);
	}
	else 
	{
		emit ui.statusBar->showMessage(QLatin1String("Datei konnte nicht geöffnet werden!"), 5000);
	}
	ui.action_Open->setEnabled(true);
}
