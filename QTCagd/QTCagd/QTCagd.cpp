#include "QTCagd.h"


QTCagd::QTCagd(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void QTCagd::closeApplication()
{
	return;
}

void QTCagd::showBunny()
{
	/*  Funktioniert nicht, da nicht im OpenGLWidget
	glewInit();

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Width of the lines.
	glLineWidth(1.0f);

	glm::vec3 eye = { 0.0f, 0.0f, 8.0f };
	glm::vec3 center = { 0.0f, 0.0f, 0.0f };
	glm::vec3 up(0.0f, 1.0f, 0.0f);

	glm::mat4x4 view = glm::lookAt(eye, center, up);

	Orb* sun = nullptr;
	char* bunny = "stanford_bunny_closed.obj";
	sun = new Orb(glm::vec3(0.0f, 0.0f, 0.0f), 0, 1.5f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	//sun->addCustomModel(bunny, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	int width = 640;
	int height = 480;
	height = height < 1 ? 1 : height;
	glViewport(0, 0, width, height);

	// Construct projection matrix.
	float zNear = 0.1f;
	float zFar = 100.0f;
	glm::mat4x4 projection = glm::perspective(45.0f, (float)width / height, zNear, zFar);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sun->render(view, projection, false, false, false);
	*/
}