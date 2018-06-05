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

void QTCagd::showSelectedVertexProperties(graphicVertex * v)
{
	selectedVertex = v;
	if (selectedVertex) {
		//TODO Enum for Modes
		ui.stackedWidget->setCurrentIndex(1);
		ui.xSpinBox->setValue(selectedVertex->location.x);
		ui.ySpinBox->setValue(selectedVertex->location.y);
		ui.zSpinBox->setValue(selectedVertex->location.z);
	}
	else {
		ui.stackedWidget->setCurrentIndex(0);
	}
}

void QTCagd::xCoordChanged(double newX)
{
	if (selectedVertex) {
		glm::vec4 oldLocation = selectedVertex->location;
		selectedVertex->location = glm::vec4(newX, oldLocation.y, oldLocation.z, oldLocation.w);
		emit vertexMoved();
	}
}

void QTCagd::yCoordChanged(double newY)
{
	if (selectedVertex) {
		glm::vec4 oldLocation = selectedVertex->location;
		selectedVertex->location = glm::vec4(oldLocation.x, newY, oldLocation.z, oldLocation.w);
		emit vertexMoved();
	}
}

void QTCagd::zCoordChanged(double newZ)
{
	if (selectedVertex) {
		glm::vec4 oldLocation = selectedVertex->location;
		selectedVertex->location = glm::vec4(oldLocation.x, oldLocation.y, newZ, oldLocation.w);
		emit vertexMoved();
	}
}
