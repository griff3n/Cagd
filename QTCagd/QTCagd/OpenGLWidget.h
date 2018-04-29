#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>

class OpenGLWidget : public QOpenGLWidget
{
public:
	OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
};
