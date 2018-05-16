#include "OpenGLWidget.h"

//Orb* sun = nullptr;
//glm::mat4x4 view;
//glm::mat4x4 projection;


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
		float scale = 10.0f;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		
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

		for (graphicFace* face : mesh->faces) {
			if (face->valence == 3) {
				halfEdge* start = face->edge;
				halfEdge* current = face->edge;
				glBegin(GL_TRIANGLES);
				for (int i = 0; i < 3; i++) {
					float x, y, z;
					x = scale * current->vert->location.x;
					y = scale * current->vert->location.y;
					z = scale * current->vert->location.z;

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
					glColor3f(factor * 1.0 + 0.0, 0.0, 0.0);

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
					x = scale * current->vert->location.x;
					y = scale * current->vert->location.y;
					z = scale * current->vert->location.z;
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	
}