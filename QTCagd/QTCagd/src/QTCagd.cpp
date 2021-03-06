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
	ui.stackedWidget->setCurrentIndex(0);
	ui.stackedWidget->setMaximumHeight(ui.Empty->maximumHeight());
	ui.stackedWidget_2->setCurrentIndex(0);
	ui.stackedWidget_2->setMaximumHeight(ui.Empty2->maximumHeight());
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
		mesh = ui.openGLWidget->getHalfEdgeMesh();
		for (graphicVertex* v : mesh->vertices) {
			file << "v " << v->location.x() << " " << v->location.y() << " " << v->location.z() << std::endl;
		}
		for (graphicFace* f : mesh->faces) {
			if (f->isHole) continue;
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

	//disable old LoDSlider
	ui.LoDSlider->setRange(0, 0);
	ui.LoDSlider->setDisabled(true);

	this->mesh = newHalfEdgeMesh;

	if (newHalfEdgeMesh)
	{
		ui.openGLWidget->setHalfEdgeMesh(newHalfEdgeMesh);
		emit ui.statusBar->showMessage(QLatin1String("Datei erfolgreich ge�ffnet!"), 5000);
	}
	else 
	{
		emit ui.statusBar->showMessage(QLatin1String("Datei konnte nicht ge�ffnet werden!"), 5000);
	}
	ui.action_Open->setEnabled(true);
	ui.limitCheckBox->setChecked(false);
	ui.smoothingSlider->setValue(0);
}

void QTCagd::showSelectedVertexProperties(graphicVertex * v)
{
	selectedVertex = v;
	if (selectedVertex) {
		//TODO Enum for Modes
		ui.stackedWidget->setCurrentIndex(1);
		ui.stackedWidget->setMaximumHeight(ui.Vertex->maximumHeight());
		ui.xSpinBox->setValue(selectedVertex->location.x());
		ui.ySpinBox->setValue(selectedVertex->location.y());
		ui.zSpinBox->setValue(selectedVertex->location.z());
		ui.wSpinBox->setValue(selectedVertex->location.w());
		ui.sharpCheckBox->setChecked(selectedVertex->sharp);
	}
	else {
		ui.stackedWidget->setCurrentIndex(0);
	}
}

void QTCagd::showSelectedHalfEdgeProperties(halfEdge *h)
{
	selectedHalfEdge = h;
	if (selectedHalfEdge) {
		//TODO Enum for Modes
		ui.stackedWidget->setCurrentIndex(2);
		ui.stackedWidget->setMaximumHeight(ui.Edge->maximumHeight());
		ui.sharpEdgeCheckBox->setChecked(selectedHalfEdge->sharp);
		if (selectedHalfEdge->face->isHole || selectedHalfEdge->pair->face->isHole) {
			ui.sharpEdgeCheckBox->setDisabled(true);
		}
		else {
			ui.sharpEdgeCheckBox->setEnabled(true);
		}
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
		ui.openGLWidget->dirtyHarry = true;
		ui.openGLWidget->dirtyDancing = true;
		emit ui.openGLWidget->repaint();
	}
}

void QTCagd::yCoordChanged(double newY)
{
	if (selectedVertex) {
		QVector4D oldLocation = selectedVertex->location;
		selectedVertex->location = QVector4D(oldLocation.x(), float(newY), oldLocation.z(), oldLocation.w());
		ui.openGLWidget->dirtyHarry = true;
		ui.openGLWidget->dirtyDancing = true;
		emit ui.openGLWidget->repaint();
	}
}

void QTCagd::zCoordChanged(double newZ)
{
	if (selectedVertex) {
		QVector4D oldLocation = selectedVertex->location;
		selectedVertex->location = QVector4D(oldLocation.x(), oldLocation.y(), float(newZ), oldLocation.w());
		ui.openGLWidget->dirtyHarry = true;
		ui.openGLWidget->dirtyDancing = true;
		emit ui.openGLWidget->repaint();
	}
}

void QTCagd::weightChanged(double w)
{
	if (selectedVertex) {
		selectedVertex->location.setW(w);
	}
}

void QTCagd::vertexMode(bool toggled)
{
	if (!toggled) return;

	ui.openGLWidget->setMode(VERTEX_MODE);
	ui.stackedWidget->setCurrentIndex(0);
	ui.stackedWidget->setMaximumHeight(ui.Empty->maximumHeight());
	if(ui.actionEdge_Mode->isChecked()) ui.actionEdge_Mode->toggle();
	if (ui.actionFace_Mode->isChecked()) ui.actionFace_Mode->toggle();
	emit ui.openGLWidget->repaint();
}

void QTCagd::edgeMode(bool toggled)
{
	if (!toggled) return;

	ui.openGLWidget->setMode(EDGE_MODE);
	ui.stackedWidget->setCurrentIndex(0);
	ui.stackedWidget->setMaximumHeight(ui.Empty->maximumHeight());
	if (ui.actionVertexMode->isChecked()) ui.actionVertexMode->toggle();
	if (ui.actionFace_Mode->isChecked()) ui.actionFace_Mode->toggle();
	emit ui.openGLWidget->repaint();
}

void QTCagd::faceMode(bool toggled)
{
	if (!toggled) return;

	ui.openGLWidget->setMode(FACE_MODE);
	ui.stackedWidget->setCurrentIndex(0);
	ui.stackedWidget->setMaximumHeight(ui.Empty->maximumHeight());
	if (ui.actionVertexMode->isChecked()) ui.actionVertexMode->toggle();
	if (ui.actionEdge_Mode->isChecked()) ui.actionEdge_Mode->toggle();
	emit ui.openGLWidget->repaint();
}

void QTCagd::catmullTool()
{
	ui.stackedWidget_2->setCurrentIndex(2);
	ui.stackedWidget_2->setMaximumHeight(ui.CatmullClark->maximumHeight());
	ui.smoothingSlider->setValue(0);
}

void QTCagd::smoothingTool()
{
	ui.stackedWidget_2->setCurrentIndex(1);
	ui.stackedWidget_2->setMaximumHeight(ui.Smoothing->maximumHeight());
	ui.limitCheckBox->setChecked(false);
}

void QTCagd::sharpVertex(bool sharp) {
	if (selectedVertex) {
		selectedVertex->sharp = sharp;
		ui.openGLWidget->dirtyHarry = true;
		emit ui.openGLWidget->repaint();
	}
}

void QTCagd::sharpEdge(bool sharp){
	if (selectedHalfEdge) {
		selectedHalfEdge->sharp = sharp;
		selectedHalfEdge->pair->sharp = sharp;
		ui.openGLWidget->dirtyHarry = true;
		emit ui.openGLWidget->repaint();
	}
}

void QTCagd::updateLoDSlider()
{
	if (!ui.limitCheckBox->isEnabled()) ui.limitCheckBox->setEnabled(true);
	if (ui.LoDSlider->isEnabled()) {
		int previousLoD = ui.LoDSlider->value();
		ui.LoDSlider->setRange(0, previousLoD + 1);
		ui.LoDSlider->setSliderPosition(previousLoD + 1);
		QString labelText = "Level of Detail: " + QString::number(ui.LoDSlider->value(), 10);
		ui.LoDLabel->setText(labelText);
	}
	else {
		ui.LoDSlider->setEnabled(true);
		ui.LoDSlider->setRange(0,1);
		ui.LoDSlider->setSliderPosition(1);
		QString labelText = "Level of Detail: " + QString::number(ui.LoDSlider->value(), 10);
		ui.LoDLabel->setText(labelText);
	}
}

void QTCagd::resetSmoothingSlider() {
	ui.smoothingSlider->setValue(0);
	QString labelText = QLatin1String("Gl�ttung: ") + QString::number(0, 10) + "%";
	ui.smoothingLabel->setText(labelText);
}

void QTCagd::updateSmoothingLabel(int level) {
	QString labelText = QLatin1String("Gl�ttung: ") + QString::number(level, 10) + "%";
	ui.smoothingLabel->setText(labelText);
}

void QTCagd::updateTextbox(QString text) {
	ui.textEdit->clear();
	ui.textEdit->setText(text);
}

void QTCagd::updateLoDLabel(int level) {
	QString labelText = "Level of Detail: " + QString::number(level, 10);
	ui.LoDLabel->setText(labelText);
}
