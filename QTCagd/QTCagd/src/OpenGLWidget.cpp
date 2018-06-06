#include "OpenGLWidget.h"
#include <Windows.h>
#include <Qt3DCore>
#include <Qt3DExtras>

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{
	delete this->mesh;
}

void OpenGLWidget::vertexMovementRepaint()
{
	emit repaint();
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
	glLineWidth(2.0f);

	QVector3D center, up;
	eye = QVector3D(0, 0, 5);
	center = QVector3D(0, 0, 0);
	up = QVector3D(0, 1, 0);
	view.setToIdentity();
    view.lookAt(eye, center, up);

	setFocusPolicy(Qt::ClickFocus);
	arcballRotationMatrix.setToIdentity();
	
}

void OpenGLWidget::paintGL()
{
	if (!mesh) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-0.5, -0.5, 0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.5, -0.5, 0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.5, 0);
		glEnd();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		modelView = view * arcballRotationMatrix * mesh->model;
		
		for (graphicVertex* vertex : mesh->vertices) {
		
			Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();

			sphereTransform->setScale(1.3f);
			sphereTransform->setTranslation(QVector3D(vertex->location.x, vertex->location.y, vertex->location.z));

			Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
			sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));

			// Sphere

			Qt3DCore::QEntity* m_sphereEntity = new Qt3DCore::QEntity();
			m_sphereEntity->addComponent(vertex->getDesign()->getActiveSkin()->returnSkinObject());
			m_sphereEntity->addComponent(sphereMaterial);
			m_sphereEntity->addComponent(sphereTransform);
			m_sphereEntity->setEnabled(true);
		}

		for (halfEdge* edge : mesh->halfEdges) {
			float x, y, z, x2, y2, z2;
			x = edge->vert->location.x;
			y = edge->vert->location.y;
			z = edge->vert->location.z;
			x2 = edge->next->vert->location.x;
			y2 = edge->next->vert->location.y;
			z2 = edge->next->vert->location.z;
			QVector4D v1 = QVector4D(x, y, z, 1);
			QVector4D v2 = QVector4D(x2, y2, z2, 1);
			v1 = modelView * v1;
			v2 = modelView * v2;
			x = v1.x();
			y = v1.y();
			z = v1.z();
			x2 = v2.x();
			y2 = v2.y();
			z2 = v2.z();
			glBegin(GL_LINES);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(x, y, z);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(x2, y2, z2);
			glEnd();
		}
		
		for (graphicFace* face : mesh->faces) {
			if (face->valence == 3) {
				halfEdge* start = face->edge;
				halfEdge* current = face->edge;
				glBegin(GL_TRIANGLES);
				for (int i = 0; i < 3; i++) {
					float x, y, z;
					x = current->vert->location.x;
					y = current->vert->location.y;
					z = current->vert->location.z;
					QVector4D location = QVector4D(x,y,z,1);
					location = modelView *location;
					x = location.x();
					y = location.y();
					z = location.z();

					//color magic
					float factor;
					glm::vec3 vertexNormal;
					halfEdge* start = current->vert->edge;
					for (int i = 0; i < current->vert->valence; i++) {
						glm::vec3 firstEdge = glm::vec3(start->next->vert->location - start->vert->location);
						glm::vec3 secondEdge = glm::vec3(start->next->next->vert->location - start->next->vert->location);
						glm::vec3 faceNormal = glm::cross(firstEdge, secondEdge);
						faceNormal = glm::normalize(faceNormal);
						vertexNormal += faceNormal;
					}
					vertexNormal = glm::normalize(vertexNormal);
					glm::vec3 antiLightDir = glm::vec3(0.0, 1.0, 1.0) - glm::vec3(0.0, 0.0, 0.0);
					glm::normalize(antiLightDir);
					factor = vertexNormal.x * antiLightDir.x + vertexNormal.y * antiLightDir.y + vertexNormal.z * antiLightDir.z;
					if (factor < 0) factor = 0;
					if(current->vert->selected) glColor3f(0.0, 1.0, 0.0);
					else glColor3f(factor * 1.0 + 0.0, 0.0, 0.0);

					glVertex3f(x, y, z);
					current = current->next;
				}
				glEnd();
			}
			else if (face->valence == 4) {
				halfEdge* start = face->edge;
				halfEdge* current = face->edge;
				glBegin(GL_QUADS);
				for (int i = 0; i < 4; i++) {
					float x, y, z;
					x = current->vert->location.x;
					y = current->vert->location.y;
					z = current->vert->location.z;

					QVector4D location = QVector4D(x, y, z, 1);
					location = modelView *location;
					x = location.x();
					y = location.y();
					z = location.z();

					glColor3f(0.0, 0.0, 1.0);
					glVertex3f(x, y, z);
					current = current->next;
				}
				glEnd();
			}
		}
	}
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
		if (!(axisYModified||axisZModified)) {
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
	case Qt::Key_Z  :
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

	if (drag && selections.size()!=0)
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

		QVector3D move = axisModifierVector *(distance * (end - begin).normalized() - distance * (endOld - beginOld).normalized());
		for (graphicVertex* v : selections) {
			QVector3D position = QVector3D(v->location.x, v->location.y, v->location.z);
			QVector3D newPosition = position + move;
			v->location.x = newPosition.x();
			v->location.y = newPosition.y();
			v->location.z = newPosition.z();
		}
		emit repaint();

		//Update SpinBoxes
		if(selections.size() == 1) emit vertexSelected(selections.at(0));

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
			qInfo() << axis;
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

void OpenGLWidget::intersect(const QVector3D &origin, const QVector3D &direction)
{
	//float minimum = std::numeric_limits<float>::max();
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
