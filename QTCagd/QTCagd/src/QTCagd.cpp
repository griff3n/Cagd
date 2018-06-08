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

	ofstream file;
	file.open(filename.toStdString());

	if (mesh) {
		for (graphicVertex* v : mesh->vertices) {
			file << "v " << v->location.x << " " << v->location.y << " " << v->location.z << std::endl;
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
