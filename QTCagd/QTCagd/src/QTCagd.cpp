#include "QTCagd.h"

/*enum OpenGLWidgetMode
{
	VERTEX_MODE,
	EDGE_MODE,
	FACE_MODE
};*/

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
	QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), "./ObjectFiles", tr("Wavefront OBJ (*.obj)"));

	if (filename.isEmpty())
		return;

	openedFile = filename;

	QFuture<HalfEdgeMesh *> future = QtConcurrent::run(loadOBJreg, filename.toStdString());

	ui.action_Open->setEnabled(false);
	emit ui.statusBar->showMessage("Datei wird eingelesen...");
	parseWatcher.setFuture(future);
}

void QTCagd::save()
{
	if (openedFile.isEmpty())
		return;

	ui.actionSave->setEnabled(false);
	ui.actionSave_as->setEnabled(false);
	emit ui.statusBar->showMessage("Datei wird gespeichert...");

	saveFile(openedFile);

	ui.actionSave->setEnabled(true);
	ui.actionSave_as->setEnabled(true);
}

void QTCagd::saveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save obj file"), "", tr("Wavefront OBJ(*.obj);;All Files (*)"));
	//QFileDialog::getOpenFileName(this, tr("Open file"), ".", tr("Wavefront OBJ (*.obj)"));

	if (filename.isEmpty())
		return;

	ui.actionSave->setEnabled(false);
	ui.actionSave_as->setEnabled(false);
	emit ui.statusBar->showMessage("Datei wird gespeichert...");

	saveFile(filename);
	openedFile = filename;

	ui.actionSave->setEnabled(true);
	ui.actionSave_as->setEnabled(true);
}

void QTCagd::saveFile(QString filename) {

	std::ofstream file;
	file.open(filename.toStdString());

	if (mesh) {
		for (graphicVertex* v : mesh->vertices) {
			file << "v " << v->location.x() << " " << v->location.y() << " " << v->location.z() << std::endl;
		}
		for (graphicFace* f : mesh->faces) {
			file << "f";
			halfEdge* start = f->edge;
			for (int i = 0; i < mesh->vertices.size(); i++) {
				if (mesh->vertices.at(i) == start->vert) {
					file << " " << i+1;
					break;
				}
			}
			halfEdge* current = f->edge->next;
			while (current != start) {
				for (int i = 0; i < mesh->vertices.size(); i++) {
					if (mesh->vertices.at(i) == current->vert) {
						file << " " << i+1;
						break;
					}
				}
				current = current->next;
			}
			file << std::endl;
		}
	}
	
	file.close();

	emit ui.statusBar->showMessage("Datei erfolgreich gespeichert!", 5000);
}

void QTCagd::parsingDone()
{
	QFuture<HalfEdgeMesh *> future = parseWatcher.future();
	HalfEdgeMesh* newHalfEdgeMesh = future.result();

	this->mesh = newHalfEdgeMesh;

	if (newHalfEdgeMesh)
	{
		ui.openGLWidget->setHalfEdgeMesh(newHalfEdgeMesh);
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
		ui.xSpinBox->setValue(selectedVertex->location.x());
		ui.ySpinBox->setValue(selectedVertex->location.y());
		ui.zSpinBox->setValue(selectedVertex->location.z());
	}
	else {
		ui.stackedWidget->setCurrentIndex(0);
	}
}

void QTCagd::xCoordChanged(double newX)
{
	if (selectedVertex) {
		QVector4D oldLocation = selectedVertex->location;
		selectedVertex->location = QVector4D(float(newX), oldLocation.y(), oldLocation.z(), oldLocation.w());
		emit ui.openGLWidget->repaint();
	}
}

void QTCagd::yCoordChanged(double newY)
{
	if (selectedVertex) {
		QVector4D oldLocation = selectedVertex->location;
		selectedVertex->location = QVector4D(oldLocation.x(), float(newY), oldLocation.z(), oldLocation.w());
		emit ui.openGLWidget->repaint();
	}
}

void QTCagd::zCoordChanged(double newZ)
{
	if (selectedVertex) {
		QVector4D oldLocation = selectedVertex->location;
		selectedVertex->location = QVector4D(oldLocation.x(), oldLocation.y(), float(newZ), oldLocation.w());
		emit ui.openGLWidget->repaint();
	}
}

void QTCagd::vertexMode(bool toggled)
{
	if (!toggled) return;

	ui.openGLWidget->setMode(VERTEX_MODE);
	ui.stackedWidget->setCurrentIndex(0);
	if(ui.actionEdge_Mode->isChecked()) ui.actionEdge_Mode->toggle();
	if (ui.actionFace_Mode->isChecked()) ui.actionFace_Mode->toggle();
}

void QTCagd::edgeMode(bool toggled)
{
	if (!toggled) return;

	ui.openGLWidget->setMode(EDGE_MODE);
	ui.stackedWidget->setCurrentIndex(0);
	if (ui.actionVertexMode->isChecked()) ui.actionVertexMode->toggle();
	if (ui.actionFace_Mode->isChecked()) ui.actionFace_Mode->toggle();
}

void QTCagd::faceMode(bool toggled)
{
	if (!toggled) return;

	ui.openGLWidget->setMode(FACE_MODE);
	ui.stackedWidget->setCurrentIndex(0);
	if (ui.actionVertexMode->isChecked()) ui.actionVertexMode->toggle();
	if (ui.actionEdge_Mode->isChecked()) ui.actionEdge_Mode->toggle();
}
