#include "OpenGLWidget.h"

Orb* sun = nullptr;
glm::mat4x4 view;
glm::mat4x4 projection;

OpenGLWidget::OpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{

}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::initializeGL()
{
	/*
	glClearColor(0.2, 0.2, 0.2, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	*/

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
	
}

void OpenGLWidget::paintGL()
{
	/*
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-0.5, -0.5, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.5, -0.5, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.5, 0);
	glEnd();
	*/

	

	

	sun->render(view, projection, false, false, false);
}

void OpenGLWidget::resizeGL(int w, int h)
{
	/*
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	*/
}