#pragma once

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
#include "SkinSphereVertex.h"
#include <QtMath>
#include "MeshTester.h"

enum OpenGLWidgetMode
{
	VERTEX_MODE,
	EDGE_MODE,
	FACE_MODE
};

class OpenGLWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();
	void setMode(OpenGLWidgetMode mode);
	void setHalfEdgeMesh(HalfEdgeMesh*);
	HalfEdgeMesh * getHalfEdgeMesh();
	bool dirtyHarry = false;
	bool dirtyDancing = false;//smoothing

public slots:
	void catmullClark();
	void deleteVertex();
	void deleteEdge();
	void deleteFace();
	void changeLoD(int);
	void smoothenMesh(int);
	void changeLimitMode(bool);
	void applySmoothing();
	
signals:
	void vertexSelected(graphicVertex*);
	void halfEdgeSelected(halfEdge*);
	void faceSelected(graphicFace*);
	void loDAdded();
	void outputChanged(QString text);

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
	void pick(const QVector2D &pos);
	QVector3D projectOntoSphere(const QPoint & pos);
	void intersect(const QVector3D& origin, const QVector3D& direction);
	void intersectVertices(const QVector3D& origin, const QVector3D& direction);
	void intersectEdges(const QVector3D& origin, const QVector3D& direction);
	void intersectFaces(const QVector3D& origin, const QVector3D& direction);
	void renderVertices();
	void renderEdges();
	void renderFaces();
	void renderNormals();
	void calculateLimitPoints();
	void testMesh();
	void calcSmoothPoints();


private:
	HalfEdgeMesh * mesh = nullptr;
	bool arcball = false;
	bool drag = false;
	bool move = false;
	bool multSelection = false;
	std::vector<graphicVertex*> selections;
	std::vector<halfEdge*> heSelections;
	std::vector<graphicFace*> fSelections;
	QMatrix4x4 arcballRotationMatrix;
	int wdth;
	int hght;
	QVector3D eye;
	QMatrix4x4 view;
	QMatrix4x4 projection;
	QMatrix4x4 modelView;
	QVector4D viewport;
	QPoint lastMousePosition;
	QPoint currentMousePosition;
	QOpenGLShaderProgram * program = new QOpenGLShaderProgram(this);
	//Booleans to save currently hold axis and reset only in the correct case
	//maybe there is an easier solution to this
	bool axisXModified = false;
	bool axisYModified = false;
	bool axisZModified = false;
	//Modification vectors to only operate on an specific axis
	QVector3D xAxisModifier = QVector3D(1, 0, 0);
	QVector3D yAxisModifier = QVector3D(0, 1, 0);
	QVector3D zAxisModifier = QVector3D(0, 0, 1);
	//Standardvector to operate on all axis. Must be set again after modification.
	QVector3D standardAxisModifier = QVector3D(1, 1, 1);
	//The current modify vector to work with.
	QVector3D axisModifierVector = standardAxisModifier;
	//Skin of the Vertices
	SkinSphereVertex *vertexSkin = new SkinSphereVertex();
	std::vector<GLfloat> skinFaces;
	//Render Mode
	OpenGLWidgetMode mode = VERTEX_MODE;
	float limitMode = 0;
};
