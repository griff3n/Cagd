#include "OpenGLWidget.h"
#include <Windows.h>

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{
	delete this->mesh;
	delete vertexSkin;
}

void OpenGLWidget::setHalfEdgeMesh(HalfEdgeMesh* mesh)
{
	//clean up existing mesh
	delete this->mesh;
	selections.clear();
	emit vertexSelected(nullptr);

	this->mesh = mesh;

	//reset View
	QVector3D center, up;
	eye = QVector3D(0, 0, 5);
	center = QVector3D(0, 0, 0);
	up = QVector3D(0, 1, 0);
	view.setToIdentity();
	view.lookAt(eye, center, up);

	//reset arcball
	arcballRotationMatrix.setToIdentity();

	emit repaint();

	//Set Focus for multSelection
	setFocus();
}

void OpenGLWidget::initializeGL()
{

	glClearColor(0.2, 0.2, 0.2, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glDepthFunc(GL_LEQUAL);
	glLineWidth(1.0f);

	glEnable(GL_PROGRAM_POINT_SIZE);

	QVector3D center, up;
	eye = QVector3D(0, 0, 5);
	center = QVector3D(0, 0, 0);
	up = QVector3D(0, 1, 0);
	view.setToIdentity();
	view.lookAt(eye, center, up);

	setFocusPolicy(Qt::ClickFocus);
	arcballRotationMatrix.setToIdentity();

	//Shader Setup
	program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shader/simple.vert");
	program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shader/simple.frag");
	program->link();
	program->bind();

	//build skinFaces Vector
	HalfEdgeMesh *skinMesh = vertexSkin->returnSkinObject();
	for (graphicFace* face : skinMesh->faces) {
		if (face->valence == 3) {
			halfEdge* start = face->edge;
			halfEdge* current = face->edge;
			for (int i = 0; i < 3; i++) {
				skinFaces.push_back(current->vert->location.x);
				skinFaces.push_back(current->vert->location.y);
				skinFaces.push_back(current->vert->location.z);
				current = current->next;
			}
		}
	}
}

void OpenGLWidget::paintGL()
{
	if (mesh) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (mode)
		{
		case VERTEX_MODE:
			renderFaces();
			renderEdges();
			renderVertices();
			break;
		case EDGE_MODE:
			renderFaces();
			renderEdges();
			break;
		case FACE_MODE:
			renderFaces();
			renderEdges();
			break;
		//default:
		}
	}
}

void OpenGLWidget::renderVertices()
{
	QColor blue(0, 0, 255, 255);
	QColor orange(239, 122, 0);

	int vertexLocation = program->attributeLocation("vertex");
	int matrixLocation = program->uniformLocation("matrix");
	int colorLocation = program->uniformLocation("color");

	QMatrix4x4 pmvMatrix;
	modelView = view * arcballRotationMatrix * mesh->model;
	pmvMatrix = projection * modelView;

	for (graphicVertex* vertex : mesh->vertices) {

		HalfEdgeMesh *skinMesh = vertexSkin->returnSkinObject();
		QMatrix4x4 scale = QMatrix4x4();
		scale.setToIdentity();
		QVector3D point = mesh->model * QVector3D(vertex->location.x, vertex->location.y, vertex->location.z);
		float distance = (eye * arcballRotationMatrix - point).length();
		//float zoomCorrection = eye.z() > 0.5 ? eye.z() : 0.5;
		scale.scale(0.005 * distance / (mesh->scale));// (mesh->scale*zoomCorrection));
		QMatrix4x4 translation = QMatrix4x4();
		translation.setToIdentity();
		translation.translate(vertex->location.x, vertex->location.y, vertex->location.z);
		QMatrix4x4 pvm = pmvMatrix * translation * scale * skinMesh->model;

		program->enableAttributeArray(vertexLocation);
		program->setAttributeArray(vertexLocation, skinFaces.data(), 3);
		program->setUniformValue(matrixLocation, pvm);
		if (vertex->selected) {
			program->setUniformValue(colorLocation, orange);
		}
		else {
			program->setUniformValue(colorLocation, blue);
		}

		int numberOfFaceVertices = skinFaces.size() / 3;
		glDrawArrays(GL_TRIANGLES, 0, numberOfFaceVertices);

		program->disableAttributeArray(vertexLocation);
	}
}
void OpenGLWidget::renderEdges()
{
	QColor green(0, 255, 0, 255);

	std::vector<GLfloat> halfEdges;

	int vertexLocation = program->attributeLocation("vertex");
	int matrixLocation = program->uniformLocation("matrix");
	int colorLocation = program->uniformLocation("color");

	QMatrix4x4 pmvMatrix;
	modelView = view * arcballRotationMatrix * mesh->model;
	pmvMatrix = projection * modelView;

	//Rendern der Edges
	for (halfEdge* edge : mesh->halfEdges) {
		halfEdges.push_back(edge->vert->location.x);
		halfEdges.push_back(edge->vert->location.y);
		halfEdges.push_back(edge->vert->location.z);
		halfEdges.push_back(edge->next->vert->location.x);
		halfEdges.push_back(edge->next->vert->location.y);
		halfEdges.push_back(edge->next->vert->location.z);
	}

	program->enableAttributeArray(vertexLocation);
	program->setAttributeArray(vertexLocation, halfEdges.data(), 3);
	program->setUniformValue(matrixLocation, pmvMatrix);
	program->setUniformValue(colorLocation, green);

	int numberOfEdgeVertices = mesh->halfEdges.size() * 2;
	glDrawArrays(GL_LINES, 0, numberOfEdgeVertices);

	program->disableAttributeArray(vertexLocation);
}
void OpenGLWidget::renderFaces()
{
	int vertexLocation = program->attributeLocation("vertex");
	int matrixLocation = program->uniformLocation("matrix");
	int colorLocation = program->uniformLocation("color");

	QMatrix4x4 pmvMatrix;
	modelView = view * arcballRotationMatrix * mesh->model;
	pmvMatrix = projection * modelView;

	std::vector<GLfloat> triangles;

	QColor red(255, 0, 0, 255);
	//QColor yellow(255, 255, 0, 255);


	//Rendern der Faces
	for (graphicFace* face : mesh->faces) {
		if (face->valence == 3 && !face->isHole) {
			halfEdge* start = face->edge;
			halfEdge* current = face->edge;
			for (int i = 0; i < 3; i++) {
				triangles.push_back(current->vert->location.x);
				triangles.push_back(current->vert->location.y);
				triangles.push_back(current->vert->location.z);
				current = current->next;
			}
		}
		else if (face->valence > 3 && !face->isHole) {
			//TODO Change Render to only create triangles
			// Create a Triangle Fan out of every face with valence higher than 3
			// Only works if all faces are convex
			halfEdge* start = face->edge;
			halfEdge* current = start->next;
			graphicVertex * triangleFanTip = start->vert;
			// Loop starts with the second Vertex and ends with the last but one to create (valence-2) triangles
			for (int i = 0; i < face->valence - 2; i++) {
				triangles.push_back(triangleFanTip->location.x);
				triangles.push_back(triangleFanTip->location.y);
				triangles.push_back(triangleFanTip->location.z);
				triangles.push_back(current->vert->location.x);
				triangles.push_back(current->vert->location.y);
				triangles.push_back(current->vert->location.z);
				triangles.push_back(current->next->vert->location.x);
				triangles.push_back(current->next->vert->location.y);
				triangles.push_back(current->next->vert->location.z);
				current = current->next;
			}
		}
	}
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.8, 0.8);

	program->enableAttributeArray(vertexLocation);
	program->setAttributeArray(vertexLocation, triangles.data(), 3);
	program->setUniformValue(matrixLocation, pmvMatrix);
	program->setUniformValue(colorLocation, red);

	int numberOfFaceVertices = triangles.size() / 3;
	glDrawArrays(GL_TRIANGLES, 0, numberOfFaceVertices);

	program->disableAttributeArray(vertexLocation);

	glDisable(GL_POLYGON_OFFSET_FILL);
}

void OpenGLWidget::resizeGL(int w, int h)
{
	wdth = w;
	hght = h;

	glViewport(0, 0, w, h);
	viewport = QVector4D(0, 0, w, h);

	projection.setToIdentity();
	projection.perspective(60.0f, (float)w / (float)h, 0.01, 5000);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//view.setToIdentity();
	//view.lookAt(eye, { 0, 0, 0 }, { 0, 1, 0 });
}
void OpenGLWidget::mousePressEvent(QMouseEvent *e)
{
	setFocus();
	lastMousePosition = currentMousePosition = e->pos();

	switch (e->button())
	{
	case Qt::RightButton:
		arcball = true;
		break;
	case Qt::MiddleButton:
		break;
	case Qt::LeftButton:
		drag = true;
		if (mesh)
		{
			if (!multSelection) {
				for (graphicVertex* v : selections) {
					v->selected = false;
				}
				selections.clear();
			}
			pick(QVector2D(lastMousePosition.x(), height() - 1 - lastMousePosition.y()));
			if (selections.size() == 1) {
				emit vertexSelected(selections.at(0));
			}
			else {
				emit vertexSelected(nullptr);
			}
		}
		break;
	}
}
void OpenGLWidget::keyPressEvent(QKeyEvent *e) {
	switch (e->key())
	{
	case Qt::Key_Control:
		multSelection = true;
		break;
	case Qt::Key_X:
		//check if any other axis is currently hold and ignore in that case
		if (!(axisYModified || axisZModified)) {
			//modify axisvector to only have x value
			axisModifierVector = xAxisModifier;
			//indicate x-axis is currently active
			axisXModified = true;
			//change to axis.selectedSkin here
		}
		break;
	case Qt::Key_Y:
		if (!(axisXModified || axisZModified)) {
			axisModifierVector = yAxisModifier;
			axisYModified = true;
		}
		break;
	case Qt::Key_Z:
		if (!(axisYModified || axisXModified)) {
			axisModifierVector = zAxisModifier;
			axisZModified = true;
		}
		break;
	}
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *e) {
	switch (e->key())
	{
	case Qt::Key_Control:
		multSelection = false;
		break;
	case Qt::Key_X:
		//check if THIS axis is hold else ignore
		if (axisXModified) {
			//reset to standard actions
			axisModifierVector = standardAxisModifier;
			//reset modified status
			axisXModified = false;
		}
		break;
	case Qt::Key_Y:
		if (axisYModified) {
			axisModifierVector = standardAxisModifier;
			axisYModified = false;
		}
		break;
	case Qt::Key_Z:
		if (axisZModified) {
			axisModifierVector = standardAxisModifier;
			axisZModified = false;
		}
		break;
	}
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::RightButton:
		arcball = false;
		break;
	case Qt::MiddleButton:
		break;
	case Qt::LeftButton:
		drag = false;
		break;
	}
}
void OpenGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	currentMousePosition = e->pos();

	if (drag && selections.size() != 0)
	{
		//TODO Clean up messy Code
		graphicVertex* lastSelected = selections.at(selections.size() - 1);

		QVector2D posOld = QVector2D(lastMousePosition.x(), height() - 1 - lastMousePosition.y());
		QVector2D pos = QVector2D(currentMousePosition.x(), height() - 1 - currentMousePosition.y());
		QRect viewp(viewport.x(), viewport.y(), viewport.z(), viewport.w());

		QVector3D beginOld = QVector3D(posOld, 0.0f).unproject(modelView, projection, viewp);
		QVector3D endOld = QVector3D(posOld, 1.0f).unproject(modelView, projection, viewp);

		QVector3D begin = QVector3D(pos, 0.0f).unproject(modelView, projection, viewp);
		QVector3D end = QVector3D(pos, 1.0f).unproject(modelView, projection, viewp);

		QVector3D selectedPos = QVector3D(lastSelected->location.x, lastSelected->location.y, lastSelected->location.z);
		QVector3D difference = selectedPos - beginOld;
		float distance = difference.length();

		QVector3D move = axisModifierVector * (distance * (end - begin).normalized() - distance * (endOld - beginOld).normalized());
		for (graphicVertex* v : selections) {
			QVector3D position = QVector3D(v->location.x, v->location.y, v->location.z);
			QVector3D newPosition = position + move;
			v->location.x = newPosition.x();
			v->location.y = newPosition.y();
			v->location.z = newPosition.z();
		}
		emit repaint();

		//Update SpinBoxes
		if (selections.size() == 1) emit vertexSelected(selections.at(0));

		lastMousePosition = currentMousePosition;
	}
	if (arcball)
	{
		currentMousePosition = e->pos();

		if (currentMousePosition != lastMousePosition)
		{
			QVector3D last = projectOntoSphere(lastMousePosition);
			QVector3D current = projectOntoSphere(currentMousePosition);
			QVector3D diff = last - current;

			float angle = 90.0f * diff.length();
			QVector3D axis = QVector3D::crossProduct(last, current);

			QMatrix4x4 arcballRotation;

			axis = axis * axisModifierVector;
			//qInfo() << axis;
			arcballRotation.setToIdentity();
			arcballRotation.rotate(angle, axis);

			lastMousePosition = currentMousePosition;
			arcballRotationMatrix = arcballRotation * arcballRotationMatrix;

			emit repaint();
		}
	}
}

QVector3D OpenGLWidget::projectOntoSphere(const QPoint& pos)
{
	QVector3D p;

	p.setX((2.0f * pos.x()) / (float)wdth - 1.0f);
	p.setY(1.0f - (2.0f * pos.y()) / (float)hght);

	float d = std::min(1.0f, p.lengthSquared());

	p.setZ(std::cosf(glm::pi<float>() * 0.5f * d));
	p.normalize();

	return p;
}

void OpenGLWidget::wheelEvent(QWheelEvent* we)
{
	QPoint numDegrees = we->angleDelta();

	eye.setZ(eye.z() * (numDegrees.y() < 0 ? 1.1f : 0.9f));

	view.setToIdentity();
	view.lookAt(eye, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0, 0.0f });

	emit repaint();
}

void OpenGLWidget::pick(const QVector2D &pos)
{
	//makeCurrent();

	// Project from 2D to 3D.
	QRect viewp(viewport.x(), viewport.y(), viewport.z(), viewport.w());

	QVector3D begin = QVector3D(pos, 0.0f).unproject(modelView, projection, viewp);
	QVector3D end = QVector3D(pos, 1.0f).unproject(modelView, projection, viewp);

	// Create ray.
	QVector3D origin = begin;
	QVector3D direction = (end - begin).normalized();

	intersect(origin, direction);

	emit repaint();
}

void OpenGLWidget::setMode(OpenGLWidgetMode mode)
{
	this->mode = mode;
	for (graphicVertex* v : selections) {
		v->selected = false;
	}
	selections.clear();
	emit vertexSelected(nullptr);
}

void OpenGLWidget::intersect(const QVector3D &origin, const QVector3D &direction)
{
	switch (mode)
	{
	case VERTEX_MODE:
		intersectVertices(origin, direction);
		break;
	case EDGE_MODE:
		intersectEdges(origin, direction);
		break;
	case FACE_MODE:
		intersectFaces(origin, direction);
		break;
	}
}

void OpenGLWidget::intersectVertices(const QVector3D& origin, const QVector3D& direction) {
	float minimum = 0.5 / mesh->scale;
	graphicVertex *closest = nullptr;
	for (graphicVertex *v : mesh->vertices) {
		QVector3D p = QVector3D(v->location.x, v->location.y, v->location.z);
		QVector3D op = p - origin;
		QVector3D cross = QVector3D::crossProduct(direction, op);

		float distance = cross.length() / direction.length();

		if (minimum > distance)
		{
			minimum = distance;
			closest = v;
		}
	}

	if (closest) {
		if (closest->selected && multSelection) {
			closest->selected = false;
			OutputDebugStringW(L"Deselected");
			for (int i = 0; i < selections.size(); i++) {
				if (selections[i] == closest) selections.erase(selections.begin() + i);
			}
		}
		else {
			closest->selected = true;
			OutputDebugStringW(L"Selected");
			selections.push_back(closest);
		}
	}
	qInfo() << "Minimum: " << minimum << "\n";
}
void OpenGLWidget::intersectEdges(const QVector3D& origin, const QVector3D& direction) {
	/*
	viscas::topology_toolbox toolbox(halfEdgeMesh);

	auto index = viscas::topology_toolbox::invalid_edge();
	auto minimum = std::numeric_limits<float>::max();

	for (auto edge : toolbox.edges())
	{
		auto v0 = toolbox.limitPointOrPoint(edge, true);
		auto v1 = toolbox.limitPointOrPoint(edge, false);

		// #todo Big n will lead to performance issues. Optimize!
		const int n = 32;

		for (int i = 0; i < n + 1; i++)
		{
			float t = (float)i / n;

			auto mid = (1.0f - t) * v0 + t * v1;
			auto distance = mid.distanceToLine(origin, direction);

			if (minimum > distance)
			{
				minimum = distance;
				index = edge;
			}
		}
	}

	if (invalid(index))
		return;

	if (append)
		selectionModel->addEdgeConditional(index);
	else
		selectionModel->setEdge(index);
    */
}
void OpenGLWidget::intersectFaces(const QVector3D& origin, const QVector3D& direction) {

}