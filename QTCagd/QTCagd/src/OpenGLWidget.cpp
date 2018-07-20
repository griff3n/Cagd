#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{
	if (this->mesh != nullptr) {
		while (this->mesh->lastLOD != nullptr) {
			this->mesh = this->mesh->lastLOD;
		}
	}
	delete this->mesh;
	delete vertexSkin;
}

void OpenGLWidget::setHalfEdgeMesh(HalfEdgeMesh* mesh)
{
	//clean up existing mesh
	if (this->mesh != nullptr) {
		while (this->mesh->lastLOD != nullptr) {
			this->mesh = this->mesh->lastLOD;
		}
	}
	delete this->mesh;
	selections.clear();
	heSelections.clear();
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

HalfEdgeMesh * OpenGLWidget::getHalfEdgeMesh()
{
	return this->mesh;
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
				skinFaces.push_back(current->vert->location.x());
				skinFaces.push_back(current->vert->location.y());
				skinFaces.push_back(current->vert->location.z());
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
	QColor darkgrey(80, 80, 80, 255);

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
		QVector3D point = mesh->model * QVector3D(vertex->location.x(), vertex->location.y(), vertex->location.z());
		float distance = (eye * arcballRotationMatrix - point).length();
		//float zoomCorrection = eye.z() > 0.5 ? eye.z() : 0.5;
		scale.scale(0.005 * distance / (mesh->scale));// (mesh->scale*zoomCorrection));
		QMatrix4x4 translation = QMatrix4x4();
		translation.setToIdentity();
		translation.translate(vertex->location.x(), vertex->location.y(), vertex->location.z());
		QMatrix4x4 pvm = pmvMatrix * translation * scale * skinMesh->model;

		program->enableAttributeArray(vertexLocation);
		program->setAttributeArray(vertexLocation, skinFaces.data(), 3);
		program->setUniformValue(matrixLocation, pvm);
		if (vertex->getIsSelected()) {
			program->setUniformValue(colorLocation, orange);
		}
		else {
			program->setUniformValue(colorLocation, darkgrey);
		}

		int numberOfFaceVertices = skinFaces.size() / 3;
		glDrawArrays(GL_TRIANGLES, 0, numberOfFaceVertices);

		program->disableAttributeArray(vertexLocation);
	}
}
void OpenGLWidget::renderEdges()
{
	QColor grey(140, 140, 140, 255);
	QColor green(0, 255, 0, 255);
	QColor orange(239, 122, 0);

	std::vector<GLfloat> halfEdges;
	std::vector<GLfloat> selHalfEdges;

	int vertexLocation = program->attributeLocation("vertex");
	int matrixLocation = program->uniformLocation("matrix");
	int colorLocation = program->uniformLocation("color");

	QMatrix4x4 pmvMatrix;
	modelView = view * arcballRotationMatrix * mesh->model;
	pmvMatrix = projection * modelView;

	//Rendern der Edges
	for (halfEdge* edge : mesh->halfEdges) {
		if (edge->getIsSelected()) {
			selHalfEdges.push_back(edge->vert->location.x());
			selHalfEdges.push_back(edge->vert->location.y());
			selHalfEdges.push_back(edge->vert->location.z());
			selHalfEdges.push_back(edge->next->vert->location.x());
			selHalfEdges.push_back(edge->next->vert->location.y());
			selHalfEdges.push_back(edge->next->vert->location.z());
		}
		else {
			halfEdges.push_back(edge->vert->location.x());
			halfEdges.push_back(edge->vert->location.y());
			halfEdges.push_back(edge->vert->location.z());
			halfEdges.push_back(edge->next->vert->location.x());
			halfEdges.push_back(edge->next->vert->location.y());
			halfEdges.push_back(edge->next->vert->location.z());
		}
	}

	program->enableAttributeArray(vertexLocation);
	program->setAttributeArray(vertexLocation, halfEdges.data(), 3);
	program->setUniformValue(matrixLocation, pmvMatrix);
	program->setUniformValue(colorLocation, grey);

	int numberOfEdgeVertices = halfEdges.size() / 3;
	glDrawArrays(GL_LINES, 0, numberOfEdgeVertices);

	program->disableAttributeArray(vertexLocation);

	//render selected Edges
	program->enableAttributeArray(vertexLocation);
	program->setAttributeArray(vertexLocation, selHalfEdges.data(), 3);
	program->setUniformValue(matrixLocation, pmvMatrix);
	program->setUniformValue(colorLocation, orange);

	numberOfEdgeVertices = selHalfEdges.size() / 3;
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

	QColor lightgrey(220, 220, 220, 255);
	QColor red(255, 0, 0, 255);
	//QColor yellow(255, 255, 0, 255);


	//Rendern der Faces
	for (graphicFace* face : mesh->faces) {
		if (face->valence == 3 && !face->isHole) {
			halfEdge* start = face->edge;
			halfEdge* current = face->edge;
			for (int i = 0; i < 3; i++) {
				triangles.push_back(current->vert->location.x());
				triangles.push_back(current->vert->location.y());
				triangles.push_back(current->vert->location.z());
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
				triangles.push_back(triangleFanTip->location.x());
				triangles.push_back(triangleFanTip->location.y());
				triangles.push_back(triangleFanTip->location.z());
				triangles.push_back(current->vert->location.x());
				triangles.push_back(current->vert->location.y());
				triangles.push_back(current->vert->location.z());
				triangles.push_back(current->next->vert->location.x());
				triangles.push_back(current->next->vert->location.y());
				triangles.push_back(current->next->vert->location.z());
				current = current->next;
			}
		}
	}
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(0.8, 0.8);

	program->enableAttributeArray(vertexLocation);
	program->setAttributeArray(vertexLocation, triangles.data(), 3);
	program->setUniformValue(matrixLocation, pmvMatrix);
	program->setUniformValue(colorLocation, lightgrey);

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
			switch (mode)
			{
			case VERTEX_MODE:
				if (!multSelection) {
					for (graphicVertex* v : selections) {
						v->setSelected(false);
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
				break;
			case EDGE_MODE:
				if (!multSelection) {
					for (halfEdge* h : heSelections) {
						h->setSelected(false);
						h->pair->setSelected(false);
					}
					heSelections.clear();
				}
				pick(QVector2D(lastMousePosition.x(), height() - 1 - lastMousePosition.y()));
				if (heSelections.size() == 1) {
					emit halfEdgeSelected(heSelections.at(0));
				}
				else {
					emit halfEdgeSelected(nullptr);
				}
				break;
			case FACE_MODE:
				break;
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
	case Qt::Key_Delete:
		deleteVertex();
		break;
	case Qt::Key_T:
		testMesh();
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

		QVector3D selectedPos = QVector3D(lastSelected->location.x(), lastSelected->location.y(), lastSelected->location.z());
		QVector3D difference = selectedPos - beginOld;
		float distance = difference.length();

		QVector3D move = axisModifierVector * (distance * (end - begin).normalized() - distance * (endOld - beginOld).normalized());
		for (graphicVertex* v : selections) {
			QVector3D position = QVector3D(v->location.x(), v->location.y(), v->location.z());
			QVector3D newPosition = position + move;
			v->location[0] = newPosition.x();
			v->location[1] = newPosition.y();
			v->location[2] = newPosition.z();
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

	p.setZ(std::cosf(M_PI * 0.5f * d));
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
		v->setSelected(false);
	}
	selections.clear();
	emit vertexSelected(nullptr);
	for (halfEdge* h : heSelections) {
		h->setSelected(false);
		h->pair->setSelected(false);
	}
	heSelections.clear();
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
		QVector3D p = QVector3D(v->location.x(), v->location.y(), v->location.z());
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
		if (closest->getIsSelected() && multSelection) {
			closest->setSelected(false);
			for (int i = 0; i < selections.size(); i++) {
				if (selections[i] == closest) selections.erase(selections.begin() + i);
			}
		}
		else {
			closest->setSelected(true);
			selections.push_back(closest);
		}
	}
	qInfo() << "Minimum: " << minimum << "\n";
}
void OpenGLWidget::intersectEdges(const QVector3D& origin, const QVector3D& direction) {
	float minimum = 0.5 / mesh->scale;
	halfEdge *closest = nullptr;
	for (halfEdge *h : mesh->halfEdges) {
		graphicVertex * start = h->vert;
		graphicVertex * end = h->next->vert;
		for (float t = 0; t <= 1; t += 0.1f) {
			QVector4D p = (1-t)*start->location + t*end->location;
			QVector3D op = QVector3D(p.x(), p.y(), p.z()) - origin;
			QVector3D cross = QVector3D::crossProduct(direction, op);

			float distance = cross.length() / direction.length();

			if (minimum > distance)
			{
				minimum = distance;
				closest = h;
			}
		}
	}

	if (closest) {
		if (closest->getIsSelected() && multSelection) {
			closest->setSelected(false);
			closest->pair->setSelected(false);
			for (int i = 0; i < heSelections.size(); i++) {
				if (heSelections[i] == closest) heSelections.erase(heSelections.begin() + i);
				else if (heSelections[i] == closest->pair) heSelections.erase(heSelections.begin() + i);
			}
		}
		else {
			closest->setSelected(true);
			closest->pair->setSelected(true);
			heSelections.push_back(closest);
		}
	}
	qInfo() << "Minimum: " << minimum << "\n";
}
void OpenGLWidget::intersectFaces(const QVector3D& origin, const QVector3D& direction) {

}

//todo: delete vertex connected to only hole faces
void OpenGLWidget::deleteVertex(){
	if (!selections.empty()) {
		//v is the Vertex that is going to be deleted, e is one outgoing halfedge of v
		graphicVertex * v = selections.at(0);
		if (v->edge == nullptr) {
			mesh->vertices.erase(std::remove(mesh->vertices.begin(), mesh->vertices.end(), v), mesh->vertices.end());
			delete v;
			selections.clear();
			emit vertexSelected(nullptr);
			emit repaint();
			return;
		}
		halfEdge * e = v->edge;
		graphicFace * hole = nullptr;

		if (e->next->next == e) {
			e->next->vert->edge = nullptr;
			bool isLastFace = true;
			for (halfEdge* h : mesh->halfEdges) {
				if (h != e && h != e->next && h->face == e->face) {
					isLastFace = false;
					e->face->edge = h;
				}
			}
			if (isLastFace) {
				mesh->faces.erase(std::remove(mesh->faces.begin(), mesh->faces.end(), e->face), mesh->faces.end());
				delete e->face;
			}
			mesh->halfEdges.erase(std::remove(mesh->halfEdges.begin(), mesh->halfEdges.end(), e->next), mesh->halfEdges.end());
			delete e->next;
			mesh->halfEdges.erase(std::remove(mesh->halfEdges.begin(), mesh->halfEdges.end(), e), mesh->halfEdges.end());
			delete e;
			mesh->vertices.erase(std::remove(mesh->vertices.begin(), mesh->vertices.end(), v), mesh->vertices.end());
			delete v;
			selections.clear();
			emit vertexSelected(nullptr);
			emit repaint();
			return;
		}
		//first step of iteration
		if (e->face != nullptr && e->face->isHole) {
	hole = e->face;
		}
		halfEdge * current = e->pair->next;
		while (current != e) {
			if (current->face->isHole) {
				hole = current->face;
				break;
			}
			current = current->pair->next;
		}
		if (!hole) {
			hole = new graphicFace();
			hole->isHole = true;
			hole->edge = e;
			mesh->faces.push_back(hole);
		}
		std::vector<halfEdge *> del;
		//first step of iteration

		std::vector<halfEdge*> toConnect;
		//helper halfedges to connect the halfedges along the hole
		halfEdge* firstEndPoly = nullptr;

		if (!(e->face == hole)) {
			mesh->faces.erase(std::remove(mesh->faces.begin(), mesh->faces.end(), e->face), mesh->faces.end());
			delete e->face;
		}
		halfEdge * temp = e->next;
		//TODO check if next line is necessary
		e->face = hole;
		toConnect.push_back(temp);
		while (temp != e) {
			temp->face = hole;
			//temp->vert->edge = temp;
			if (temp->next->next == e) firstEndPoly = temp;
			temp = temp->next;
		}
		del.push_back(e);
		del.push_back(e->pair);

		current = e->pair->next;


		while (current != e) {
			if (!(current->face == hole)) {
				mesh->faces.erase(std::remove(mesh->faces.begin(), mesh->faces.end(), current->face), mesh->faces.end());
				delete current->face;
			}
			halfEdge * temp = current->next;
			while (temp != current) {
				temp->face = hole;
				//temp->vert->edge = temp;
				if (temp->next->next == current) toConnect.push_back(temp);
				temp = temp->next;
			}
			toConnect.push_back(current->next);
			del.push_back(current);
			del.push_back(current->pair);
			current = current->pair->next;
		}
		toConnect.push_back(firstEndPoly);

		for (int i = 0; i < toConnect.size(); i = i + 2) {
			halfEdge * start = toConnect.at(i);
			halfEdge * end = toConnect.at(i + 1);
			end->next = start;
		}
		/*current = e->next;
		do {
			current->vert->edge = current;
			current = current->next;
			qInfo() << "HI \n";
		} while (current != e->next);
		*/
		for (halfEdge * he : del) {
			if (he->face->isHole && he->face->edge == he) {
				for (halfEdge* h : mesh->halfEdges) {
					if (h != he && h->face == he->face) {
						he->face->edge = h;
					}
				}
			}
			if (he->vert->edge == he) {
				bool hasOutgoing = false;
				for (halfEdge* h : mesh->halfEdges) {
					if (h != he && h->vert == he->vert) {
						he->vert->edge = h;
						hasOutgoing = true;
					}
				}
				if (!hasOutgoing) he->vert->edge = nullptr;
			}
			/*
			graphicVertex* v = he->vert;
			if (v->edge == he) {
				if (he == he->pair->next) {
					v->edge = nullptr;
				}
				else {
					v->edge = he->pair->next;
				}
			}
			*/
			mesh->halfEdges.erase(std::remove(mesh->halfEdges.begin(), mesh->halfEdges.end(), he), mesh->halfEdges.end());
			delete he;
		}
		hole->valence = 2;
		mesh->vertices.erase(std::remove(mesh->vertices.begin(), mesh->vertices.end(), v), mesh->vertices.end());
		delete v;
		selections.clear();
		emit vertexSelected(nullptr);
		emit repaint();
	}
}

void OpenGLWidget::catmullClark() {
	HalfEdgeMesh * newMesh = new HalfEdgeMesh();
	newMesh->lastLOD = mesh;
	mesh->nextLOD = newMesh;
	newMesh->LOD = mesh->LOD + 1;
	newMesh->model = mesh->model;
	newMesh->scale = mesh->scale;
	for (graphicFace * f : mesh->faces) {
		if (f->isHole) {
			continue;
		}
		halfEdge * current = f->edge;
		QVector4D locF = QVector4D(0, 0, 0, 0);
		for (int i = 0; i < f->valence; i++) {
			locF += current->vert->location;
			current = current->next;
		}
		locF /= (float)f->valence;
		graphicVertex * newFaceV = new graphicVertex(locF);
		f->nextLOD = newFaceV;
		newFaceV->lastLOD = f;
		newFaceV->valence = f->valence;
		newMesh->vertices.push_back(newFaceV);
	}
	for (halfEdge * h : mesh->halfEdges) {
		if (h->pair->nextLOD == nullptr) {
			QVector4D locE = QVector4D(0, 0, 0, 0);
			if (!h->sharp) {
				locE += h->vert->location;
				locE += h->pair->vert->location;
				locE += h->face->nextLOD->location;
				locE += h->pair->face->nextLOD->location;
				locE /= 4.0;
			}
			else {
				locE += h->vert->location;
				locE += h->pair->vert->location;
				locE /= 2.0;
			}
			graphicVertex * newEdgeV = new graphicVertex(locE);
			h->nextLOD = newEdgeV;
			newEdgeV->lastLOD = h;
			if (h->face->isHole || h->pair->face->isHole) {
				newEdgeV->valence = 3;
			}
			else {
				newEdgeV->valence = 4;
			}
			newMesh->vertices.push_back(newEdgeV);
		}
		else {
			h->nextLOD = h->pair->nextLOD;
		}
	}
	for (graphicVertex * v : mesh->vertices) {
		bool sharpEdgeRule = false;
		bool cornerRule = false;
		halfEdge * current = v->edge;
		int incidentSharpEdges = 0;
		for (int i = 0; i < v->valence; i++) {
			if (current->sharp) {
				incidentSharpEdges++;
			}
			current = current->pair->next;
		}
		if (incidentSharpEdges == 2) {
			sharpEdgeRule = true;
		}
		if (incidentSharpEdges > 2 || v->sharp) {
			cornerRule = true;
		}
		QVector4D locV = QVector4D(0, 0, 0, 0);
		QVector4D q = QVector4D(0, 0, 0, 0);
		QVector4D r = QVector4D(0, 0, 0, 0);
		current = v->edge;
		if (sharpEdgeRule) {
			for (int i = 0; i < v->valence; i++) {
				//if (v->edge->sharp) {
				if (current->sharp) {
					r += current->pair->vert->location;
				}
				current = current->pair->next;
			}
			r /= 2.0;
			locV = (r + 3 * v->location)/4.0;
		}
		else if (cornerRule) {
			locV = v->location;
		}
		else {
			for (int i = 0; i < v->valence; i++) {
				//r += current->nextLOD->location;
				r += current->pair->vert->location;
				q += current->face->nextLOD->location;
				current = current->pair->next;
			}
			q /= (float)v->valence;
			r /= (float)v->valence;
			locV = (q + r)/ (float)v->valence + v->location * (1 - 2 / (float)v->valence);
		}
		graphicVertex * newV = new graphicVertex(locV);
		v->nextLOD = newV;
		if (v->sharp) newV->sharp = true;
		newV->lastLOD = v;
		newV->valence = v->valence;
		newMesh->vertices.push_back(newV);
	}
	//Verbinden der neuen Vertices
	
	for (graphicFace * f : mesh->faces) {
		if (f->isHole) {
			continue;
		}
		halfEdge * current = f->edge;
		halfEdge * lastPair;
		halfEdge * endPair;
		for (int i = 0; i < f->valence; i++) {
			graphicFace * newFace = new graphicFace();
			halfEdge * newEdge1 = new halfEdge();
			halfEdge * newEdge2 = new halfEdge();
			halfEdge * newEdge3 = new halfEdge();
			halfEdge * newEdge4 = new halfEdge();
			newEdge1->next = newEdge2;
			newEdge2->next = newEdge3;
			newEdge3->next = newEdge4;
			newEdge4->next = newEdge1;
			newEdge1->vert = current->vert->nextLOD;
			newEdge1->sharp = current->sharp;
			newEdge2->vert = current->nextLOD;
			newEdge3->vert = current->face->nextLOD;
			halfEdge * cur = current;
			for (int j = 0; j < f->valence - 1; j++) {
				cur = cur->next;
			}
			newEdge4->vert = cur->nextLOD;
			newEdge4->sharp = cur->sharp;
			newEdge1->face = newFace;
			newEdge2->face = newFace;
			newEdge3->face = newFace;
			newEdge4->face = newFace;
			newFace->edge = newEdge1;
			newFace->valence = 4;
			if (newEdge1->vert->edge == nullptr) newEdge1->vert->edge = newEdge1;
			if (newEdge2->vert->edge == nullptr) newEdge2->vert->edge = newEdge2;
			if (newEdge3->vert->edge == nullptr) newEdge3->vert->edge = newEdge3;
			if (newEdge4->vert->edge == nullptr) newEdge4->vert->edge = newEdge4;
			//pairs setzen
			if (i == 0) {
				endPair = newEdge3;
			}
			else {
				newEdge3->pair = lastPair;
				lastPair->pair = newEdge3;
			}
			if (i == f->valence) {
				newEdge2->pair = endPair;
				endPair->pair = newEdge2;
			}
			lastPair = newEdge2;

			newMesh->halfEdges.push_back(newEdge1);
			newMesh->halfEdges.push_back(newEdge2);
			newMesh->halfEdges.push_back(newEdge3);
			newMesh->halfEdges.push_back(newEdge4);
			newMesh->faces.push_back(newFace);
			current = current->next;
		}
	}
	mesh = newMesh;
	//restliche pairs setzen
	for (halfEdge * h : mesh->halfEdges) {
		if (h->pair == nullptr) {
			for (halfEdge * h2 : mesh->halfEdges) {
				if (h->next->vert == h2->vert && h2->next->vert == h->vert) {
					h->pair = h2;
					h2->pair = h;
				}
			}
		}
	}

	//hole faces erstellen, sharp edges setzen
	for (int i = 0; i < mesh->halfEdges.size(); i++) {
		halfEdge * he = mesh->halfEdges.at(i);
		if (he->pair == nullptr) {
			he->sharp = true;
			graphicFace * hole = new graphicFace;
			hole->isHole = true;
			halfEdge *holeEdge = new halfEdge;
			he->pair = holeEdge;
			holeEdge->pair = he;
			holeEdge->vert = he->next->vert;
			holeEdge->face = hole;
			holeEdge->sharp = true;
			hole->edge = holeEdge;
			mesh->faces.push_back(hole);
			halfEdge * current = he;
			halfEdge *lastHoleEdge = holeEdge;
			while (current->next != he) {
				current = current->next;
				if (current->pair == nullptr) {
					halfEdge *newHoleEdge = new halfEdge;
					current->pair = newHoleEdge;
					current->sharp = true;
					newHoleEdge->pair = current;
					newHoleEdge->vert = current->next->vert;
					newHoleEdge->face = hole;
					newHoleEdge->next = lastHoleEdge;
					newHoleEdge->sharp = true;
					lastHoleEdge = newHoleEdge;
					mesh->halfEdges.push_back(newHoleEdge);
				}
				else current = current->pair;
			}
			holeEdge->next = lastHoleEdge;
			mesh->halfEdges.push_back(holeEdge);
			//Hole Valence
			int v = 1;
			current = holeEdge;
			while (current->next != holeEdge) {
				v++;
				current = current->next;
			}
			hole->valence = v;
		}
	}
	testMesh();
	emit repaint();
}





void OpenGLWidget::testMesh() {
	MeshTester * m = new MeshTester();
	m->testMesh(mesh);
	delete m;
	m = nullptr;
	
	qInfo() << "Statistik =========================================\n";
	qInfo() << "Vertices: " << mesh->vertices.size() << "\n";
	std::vector<int> vertexValences;
	for (int i = 0; i < mesh->vertices.size(); i++) {
		if (mesh->vertices[i]->valence + 1 > vertexValences.size()) vertexValences.resize(mesh->vertices[i]->valence + 1);
		vertexValences[mesh->vertices[i]->valence] ++;
	}
	for (int i = 0; i < vertexValences.size(); i++) {
		if (vertexValences[i] > 0) {
			qInfo() << "Vertex-Valenz " << i << ": " << vertexValences[i] << "\n";
		}
	}
	int holeFaces = 0;
	for (graphicFace* f : mesh->faces) {
		if (f->isHole) holeFaces++;
	}
	qInfo() << "Faces: " << mesh->faces.size() - holeFaces << "\n";
	qInfo() << "Holes: " << holeFaces << "\n";
	std::vector<int> faceValences;
	std::vector<int> holeValences;
	for (int i = 0; i < mesh->faces.size(); i++) {
		if (mesh->faces[i]->isHole) {
			if (mesh->faces[i]->valence + 1 > holeValences.size()) holeValences.resize(mesh->faces[i]->valence + 1);
			holeValences[mesh->faces[i]->valence] ++;
		}
		else {
			if (mesh->faces[i]->valence + 1 > faceValences.size()) faceValences.resize(mesh->faces[i]->valence + 1);
			faceValences[mesh->faces[i]->valence] ++;
		}
	}
	for (int i = 0; i < faceValences.size(); i++) {
		if (faceValences[i] > 0) {
			qInfo() << "Face-Valenz " << i << ": " << faceValences[i] << "\n";
		}
	}
	for (int i = 0; i < holeValences.size(); i++) {
		if (holeValences[i] > 0) {
			qInfo() << "Hole-Valenz " << i << ": " << holeValences[i] << "\n";
		}
	}
	int sharpEdges = 0;
	for (halfEdge* h : mesh->halfEdges) {
		if (h->sharp) sharpEdges++;
	}
	qInfo() << "Halfedges: " << mesh->halfEdges.size() << "\n";
	qInfo() << "Sharp Edges: " << sharpEdges << "\n";
	qInfo() << "===================================================\n";
	qInfo() << "\n";
}