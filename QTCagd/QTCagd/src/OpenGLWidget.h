#pragma once

#include <GL/glew.h>
#include <QWidget>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
//#include <QOpenGLShader>
#include "ObjectLoader.h"
#include "HalfEdgeMesh.h"
#include <Windows.h>

#include <QtCore/QObject>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

class OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();
	

public slots:
	void setHalfEdgeMesh(HalfEdgeMesh*);
	void vertexMovementRepaint();
signals:
	void vertexSelected(graphicVertex*);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* we) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent * e) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent * e) Q_DECL_OVERRIDE;

private:
	HalfEdgeMesh * mesh = nullptr;
	bool arcball = false;
	bool drag = false;
	bool multSelection = false;
	std::vector<graphicVertex*> selections;
	QMatrix4x4 arcballRotationMatrix;
	int wdth;
	int hght;
	QPoint lastMousePosition;
	QPoint currentMousePosition;
	QVector3D eye;
	QMatrix4x4 view;
	QMatrix4x4 projection;
	QMatrix4x4 modelView;
	QVector4D viewport;
	QOpenGLShaderProgram * program = new QOpenGLShaderProgram(this);;
	//Booleans to save currently hold axis and reset only in the correct case
	//maybe there is an easier solution to this
	bool axisXModified=false;
	bool axisYModified=false;
	bool axisZModified=false;
	//Modification vectors to only operate on an specific axis
	QVector3D xAxisModifier = QVector3D(1, 0, 0);
	QVector3D yAxisModifier = QVector3D(0, 1, 0);
	QVector3D zAxisModifier = QVector3D(0, 0, 1);
	//Standardvector to operate on all axis. Must be set again after modification.
	QVector3D standardAxisModifier = QVector3D(1, 1, 1);
	//The current modify vector to work with.
	QVector3D axisModifierVector = standardAxisModifier;
	void intersect(const QVector3D& origin, const QVector3D& direction);
	QVector3D projectOntoSphere(const QPoint & pos);
	void pick(const QVector2D &pos);
};
