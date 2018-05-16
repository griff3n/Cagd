#pragma once

#include <GL/glew.h>
#include <QWidget>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>
#include "ObjectLoader.h"
#include "Orb.h"
#include "HalfEdgeMesh.h"

class OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();
	

public slots:
	void setHalfEdgeMesh(HalfEdgeMesh*);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	HalfEdgeMesh * mesh = nullptr;
};
