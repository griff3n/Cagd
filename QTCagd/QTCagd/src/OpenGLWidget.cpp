#include "OpenGLWidget.h"
#include <Windows.h>

//Orb* sun = nullptr;
//glm::mat4x4 view;
//glm::mat4x4 projection;
std::vector<graphicVertex*> selections;

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::setHalfEdgeMesh(HalfEdgeMesh* mesh)
{
	this->mesh = mesh;
	emit repaint();
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

	QVector3D eye, center, up;
	eye = QVector3D(0, 0, 5);
	center = QVector3D(0, 0, 0);
	up = QVector3D(0, 1, 0);
	view.setToIdentity();
    view.lookAt(eye, center, up);

	/*
	glewInit();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Width of the lines.
	glLineWidth(1.0f);

	glm::vec3 eye = { 0.0f, 0.0f, 4.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	view = glm::lookAt(eye, center, up);


	char* bunny = "stanford_bunny_closed.obj";
	sun = new Orb(glm::vec3(0.0f, 0.0f, 0.0f), 0, 1.5f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	sun->addCustomModel(bunny, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	int width = 640;
	int height = 480;
	height = height < 1 ? 1 : height;
	glViewport(0, 0, width, height);

	// Construct projection matrix.
	float zNear = 0.1f;
	float zFar = 100.0f;
	projection = glm::perspective(45.0f, (float)width / height, zNear, zFar);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*/
	
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
		
		modelView = view * mesh->model;
		/*
		for (halfEdge* edge : mesh->halfEdges) {
			float x, y, z, x2, y2, z2;
			x = scale * edge->vert->location.x;
			y = scale * edge->vert->location.y;
			z = scale * edge->vert->location.z;
			x2 = scale * edge->next->vert->location.x;
			y2 = scale * edge->next->vert->location.y;
			z2 = scale * edge->next->vert->location.z;
			glBegin(GL_LINES);
			glColor3f(0.0, 1.0, 1.0);
			glVertex3f(x, y, z);
			glColor3f(0.0, 1.0, 1.0);
			glVertex3f(x2, y2, z2);
			glEnd();
		}
		*/
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
	//sun->render(view, projection, false false, false);
}

void OpenGLWidget::resizeGL(int w, int h)
{
	
	glViewport(0, 0, w, h);
	viewport = QVector4D(0, 0, w, h);

	projection.setToIdentity();
	projection.perspective(60.0f, (float)w / (float)h, 0.01, 5000);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w / h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	view.setToIdentity();
	view.lookAt(QVector3D(0, 0, 5), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
}
void OpenGLWidget::mousePressEvent(QMouseEvent *e)
{
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
			auto append = e->modifiers() && Qt::ControlModifier;
			for (graphicVertex* v : selections) {
				v->selected = false;
			}
			selections.clear();
			pick(QVector2D(lastMousePosition.x(), height() - 1 - lastMousePosition.y()), append);
		}
		break;
	}

}
void OpenGLWidget::mouseReleaseEvent(QMouseEvent *e)
{

}
void OpenGLWidget::mouseMoveEvent(QMouseEvent *e)
{

}
void OpenGLWidget::wheelEvent(QWheelEvent* we)
{

}

void OpenGLWidget::pick(const QVector2D &pos, bool append)
{
	//makeCurrent();

	// Project from 2D to 3D.
	QRect viewp(viewport.x(), viewport.y(), viewport.z(), viewport.w());

	auto begin = QVector3D(pos, 0.0f).unproject(modelView, projection, viewp);
	auto end = QVector3D(pos, 1.0f).unproject(modelView, projection, viewp);

	// Create ray.
	QVector3D origin = begin;
	QVector3D direction = (end - begin).normalized();

	intersect(origin, direction, append);

	emit repaint();
}

void OpenGLWidget::intersect(const QVector3D &origin, const QVector3D &direction, bool append)
{
	float minimum = std::numeric_limits<float>::max();
	graphicVertex *closest = nullptr;
	for (graphicVertex *v : mesh->vertices) {
		QVector3D p = QVector3D(v->location.x, v->location.y, v->location.z);
		QVector3D op = p - origin;
		QVector3D cross = QVector3D::crossProduct(direction,op);
		
		float distance = cross.length() / direction.length();

		if (minimum > distance)
		{
			minimum = distance;
			closest = v;
		}
	}
	/*if (closest->selected) { 
		closest->selected = false;
		OutputDebugStringW(L"Deselected");
	}
	else {
		closest->selected = true;
		OutputDebugStringW(L"Selected");
	}
	*/
	closest->selected = true;
	selections.push_back(closest);
	float x = origin.x();
	qInfo() << "Selected origin " << origin.x() << " " << origin.y() << " " << origin.z() << "\n";
	qInfo() << "Selected direction " << direction.x() << " " << direction.y() << " " << direction.z();
}
