#pragma once

#include <GL/glew.h>
#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <QMouseEvent>
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
	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* we) Q_DECL_OVERRIDE;

private:
	HalfEdgeMesh * mesh = nullptr;
	bool arcball;
	bool drag;
	QPoint lastMousePosition;
	QPoint currentMousePosition;
	QMatrix4x4 arcballRotation;
	QVector3D eye;
	QMatrix4x4 view;
	QMatrix4x4 projection;
	QMatrix4x4 modelView;
	QVector4D viewport;
	void intersect(const QVector3D& origin, const QVector3D& direction, bool append);
	void pick(const QVector2D &pos, bool append);
};
