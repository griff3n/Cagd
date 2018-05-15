#pragma once

#include <GL/glew.h>
#include <QWidget>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>
#include "ObjectLoader.h"
#include "Orb.h"

#include "ObjectMemory.h"

#include "typedef.h"
#include "normalrenderer.h"
#include "vertexrenderer.h"
//#include "renderer/facerenderer.h"
//#include "renderer/edgerenderer.h"
//#include "renderer/visualizationrenderer.h"

#include "selectionmodel.h"
#include "propertiesmodel.h"


#include <memory>

#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector2D>
#include <QOpenGLShaderProgram>

enum OpenGLWidgetMode
{
	VISUALIZATION_MODE,
	VERTEX_MODE,
	EDGE_MODE,
	FACE_MODE
};

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();

	//void setHalfEdgeMesh(viscas::shared_half_edge_mesh& halfEdgeMesh);
	void setMode(OpenGLWidgetMode mode);
	//void setSelectionModel(viscas::shared_selection_model selectionModel);
	//void setPropertiesModel(viscas::shared_properties_model propertiesModel);

protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

	void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* we) Q_DECL_OVERRIDE;

private:
	void info();
	QVector3D projectOntoSphere(const QPoint &pos);
	void pick(const QVector2D& pos, bool append);
	void intersect(const QVector3D& origin, const QVector3D& direction, bool append);
	void intersectEdges(const QVector3D& origin, const QVector3D& direction, bool append);
	void intersectFaces(const QVector3D& origin, const QVector3D& direction, bool append);
private:
	viscas::shared_half_edge_mesh halfEdgeMesh;
	viscas::shared_selection_model selectionModel;
	viscas::shared_properties_model propertiesModel;


	// OpenGL stuff

	QMatrix4x4 view;
	QMatrix4x4 proj;
	QMatrix4x4 model;

	QMatrix4x4 modelView;

	QVector3D eye;

	QVector4D viewport;
	QVector2D nearFar;

	QColor background;

	// arcball stuff ->

	bool arcball;
	QPoint lastMousePosition;
	QPoint currentMousePosition;
	QMatrix4x4 arcballRotation;

	// <- arcball stuff

	// renderer

	// This one will be shared among all other renderer.
	viscas::shared_normal_renderer normalRenderer;

	viscas::unique_vertex_renderer vertexRenderer;
	//viscas::unique_face_renderer faceRenderer;
	//viscas::unique_edge_renderer edgeRenderer;
	//viscas::unique_visualization_renderer visRenderer;

	// Mode needed to say which renderer should be active right now.
	OpenGLWidgetMode mode;
};
