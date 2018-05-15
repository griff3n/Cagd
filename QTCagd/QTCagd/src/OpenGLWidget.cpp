#define _USE_MATH_DEFINES
#include <cmath>
#include <QSettings>
#include <QColor>
#include <QMouseEvent>

#include "OpenGLWidget.h"
#include "renderenvironment.h"
#include "typedef.h"


OpenGLWidget::OpenGLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
	, nearFar(0.01f, 1000.0f)
	, arcball(false)
	, mode(VISUALIZATION_MODE)
{
	setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

OpenGLWidget::~OpenGLWidget()
{

}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	info();

	// background
	QSettings settings;
	background = settings.value("background-color", QColor(192, 192, 192)).value<QColor>();
	glClearColor(background.redF(), background.greenF(), background.blueF(), background.alphaF());

	// camera settings
	eye = { 0.0f, 0.0f, 8.0f };

	view.setToIdentity();
	view.lookAt(eye, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0, 0.0f });

	// model
	model.setToIdentity();

	// etc.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Initialize renderer
	normalRenderer = std::shared_ptr<viscas::NormalRenderer>(std::make_shared<viscas::NormalRenderer>());

	//visRenderer = std::unique_ptr<viscas::VisualizationRenderer>(std::make_unique<viscas::VisualizationRenderer>());
	vertexRenderer = std::unique_ptr<viscas::VertexRenderer>(std::make_unique<viscas::VertexRenderer>());
	//edgeRenderer = std::unique_ptr<viscas::EdgeRenderer>(std::make_unique<viscas::EdgeRenderer>());
	//faceRenderer = std::unique_ptr<viscas::FaceRenderer>(std::make_unique<viscas::FaceRenderer>());

	normalRenderer->setSelectionModel(selectionModel);
	normalRenderer->setPropertiesModel(propertiesModel);

	/*visRenderer->setSelectionModel(selectionModel);
	visRenderer->setPropertiesModel(propertiesModel);
	visRenderer->setNormalRenderer(normalRenderer);*/

	vertexRenderer->setSelectionModel(selectionModel);
	vertexRenderer->setPropertiesModel(propertiesModel);
	vertexRenderer->setNormalRenderer(normalRenderer);

	/*edgeRenderer->setSelectionModel(selectionModel);
	edgeRenderer->setPropertiesModel(propertiesModel);
	edgeRenderer->setNormalRenderer(normalRenderer);*/

	/*faceRenderer->setSelectionModel(selectionModel);
	faceRenderer->setPropertiesModel(propertiesModel);
	faceRenderer->setNormalRenderer(normalRenderer);*/
}

void OpenGLWidget::info()
{
	QSurfaceFormat format = QSurfaceFormat::defaultFormat();
	QString profile = "";

	switch (format.profile())
	{
	case QSurfaceFormat::NoProfile:
		profile = "No Profile";
		break;
	case QSurfaceFormat::CoreProfile:
		profile = "Core";
		break;
	case QSurfaceFormat::CompatibilityProfile:
		profile = "Compatibility";
		break;
	default:
		profile = "Unkown Profile";
	}

	qInfo().nospace().noquote() << "OpenGL Version: OpenGL " << format.majorVersion() << "." << format.minorVersion() << " " << profile;
}

void OpenGLWidget::resizeGL(int w, int h)
{
	h = h < 1 ? 1 : h;

	glViewport(0, 0, w, h);
	viewport = QVector4D(0, 0, w, h);

	proj.setToIdentity();
	proj.perspective(60.0f, (float)w / (float)h, nearFar.x(), nearFar.y());

	normalRenderer->resize(w, h);
	//visRenderer->resize(w, h);
	vertexRenderer->resize(w, h);
	//edgeRenderer->resize(w, h);
	//faceRenderer->resize(w, h);
}

void OpenGLWidget::paintGL()
{
	modelView = view * model;
	auto mvp = proj * modelView;
	auto nm = model.normalMatrix();

	viscas::RenderEnvironment environment{ mvp, modelView, nm, viewport, nearFar, background };

	switch (mode)
	{
	case VISUALIZATION_MODE:
		//visRenderer->render(environment);
		break;
	case VERTEX_MODE:
		vertexRenderer->render(environment);
		break;
	case EDGE_MODE:
		//edgeRenderer->render(environment);
		break;
	case FACE_MODE:
		//faceRenderer->render(environment);
		break;
	}
}

void OpenGLWidget::wheelEvent(QWheelEvent *we)
{
	auto numDegrees = we->angleDelta();

	eye.setZ(eye.z() * (numDegrees.y() < 0 ? 1.1f : 0.9f));

	view.setToIdentity();
	view.lookAt(eye, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0, 0.0f });

	emit repaint();
}

//void OpenGLWidget::setHalfEdgeMesh(viscas::shared_half_edge_mesh& halfEdgeMesh)
//{
//	makeCurrent();
//
//	this->halfEdgeMesh = halfEdgeMesh;
//
//	normalRenderer->setHalfEdgeMesh(halfEdgeMesh);
//	vertexRenderer->setHalfEdgeMesh(halfEdgeMesh);
//	visRenderer->setHalfEdgeMesh(halfEdgeMesh);
//	edgeRenderer->setHalfEdgeMesh(halfEdgeMesh);
//	faceRenderer->setHalfEdgeMesh(halfEdgeMesh);
//
//	emit repaint();
//}

void OpenGLWidget::setMode(OpenGLWidgetMode mode)
{
	this->mode = mode;

	emit repaint();
}

//void OpenGLWidget::setSelectionModel(viscas::shared_selection_model selectionModel)
//{
//	this->selectionModel = selectionModel;
//}
//
//void OpenGLWidget::setPropertiesModel(viscas::shared_properties_model propertiesModel)
//{
//	this->propertiesModel = propertiesModel;
//
//	connect(this->propertiesModel.get(), SIGNAL(propertyChanged()), this, SLOT(repaint()));
//}

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
		if (halfEdgeMesh)
		{
			auto append = e->modifiers() && Qt::ControlModifier;
			pick(QVector2D(lastMousePosition.x(), height() - 1 - lastMousePosition.y()), append);
		}
		break;
	}
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::RightButton:
		arcball = false;
		break;
	case Qt::MiddleButton:
		break;
	case Qt::LeftButton:
		break;
	}
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (arcball)
	{
		currentMousePosition = e->pos();

		if (currentMousePosition != lastMousePosition)
		{
			QVector3D last = projectOntoSphere(lastMousePosition);
			QVector3D current = projectOntoSphere(currentMousePosition);
			QVector3D diff = last - current;

			float angle = 90.0f * diff.length();
			QVector3D axis = QVector3D::crossProduct(last, current);

			arcballRotation.setToIdentity();
			arcballRotation.rotate(angle, axis);

			lastMousePosition = currentMousePosition;
			model = arcballRotation * model;

			emit repaint();
		}
	}
}

QVector3D OpenGLWidget::projectOntoSphere(const QPoint& pos)
{
	QVector3D p;

	p.setX((2.0f * pos.x()) / (float)width() - 1.0f);
	p.setY(1.0f - (2.0f * pos.y()) / (float)height());

	float d = std::min(1.0f, p.lengthSquared());

	p.setZ(std::cosf(M_PI * 0.5f * d));
	p.normalize();

	return p;
}

void OpenGLWidget::pick(const QVector2D &pos, bool append)
{
	makeCurrent();

	// Project from 2D to 3D.
	QRect viewp(viewport.x(), viewport.y(), viewport.z(), viewport.w());

	auto begin = QVector3D(pos, 0.0f).unproject(modelView, proj, viewp);
	auto end = QVector3D(pos, 1.0f).unproject(modelView, proj, viewp);

	// Create ray.
	QVector3D origin = begin;
	QVector3D direction = (end - begin).normalized();

	intersect(origin, direction, append);

	emit repaint();
}

void OpenGLWidget::intersect(const QVector3D &origin, const QVector3D &direction, bool append)
{
	switch (mode)
	{
	case VERTEX_MODE:
		break;
	case EDGE_MODE:
		intersectEdges(origin, direction, append);
		break;
	case FACE_MODE:
		intersectFaces(origin, direction, append);
		break;
	}
}

void OpenGLWidget::intersectEdges(const QVector3D &origin, const QVector3D &direction, bool append)
{
	viscas::topology_toolbox toolbox(halfEdgeMesh);

	auto index = viscas::topology_toolbox::invalid_edge();
	auto minimum = std::numeric_limits<float>::max();

	for (auto edge : toolbox.edges())
	{
		auto v0 = toolbox.limitPointOrPoint(edge, true);
		auto v1 = toolbox.limitPointOrPoint(edge, false);

		// #todo Big n will lead to performance issues. Optimize!
		const int n = 32;

		for (int i = 0; i < n + 1; i++)
		{
			float t = (float)i / n;

			auto mid = (1.0f - t) * v0 + t * v1;
			auto distance = mid.distanceToLine(origin, direction);

			if (minimum > distance)
			{
				minimum = distance;
				index = edge;
			}
		}
	}

	if (invalid(index))
		return;

	if (append)
		selectionModel->addEdgeConditional(index);
	else
		selectionModel->setEdge(index);
}

bool rayPlaneIntersection(const QVector3D& rayOrigin, const QVector3D& rayDirection, const QVector3D& planeOrigin, const QVector3D& planeNormal, QVector3D& result, float& t)
{
	float ln = QVector3D::dotProduct(rayDirection, planeNormal);

	if (std::fabs(ln) < std::numeric_limits<float>::epsilon())
	{
		float temp = QVector3D::dotProduct((planeOrigin - rayOrigin), planeNormal);

		if (std::fabs(temp) < std::numeric_limits<float>::epsilon())
		{
			result = rayOrigin;

			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		float temp = QVector3D::dotProduct((planeOrigin - rayOrigin), planeNormal);
		t = temp / ln;

		result = rayOrigin + t * rayDirection;

		return true;
	}
}

/*
The given face will be triangulated. The result is a vector with points. For every triangle there are three points.
*/
std::vector<viscas::half_edge_mesh::point_type> triangulateToPoints(const viscas::shared_half_edge_mesh& halfEdgeMesh, viscas::half_edge_mesh::face_index_type face)
{
	std::vector<viscas::half_edge_mesh::point_type> result;
	viscas::topology_toolbox toolbox(halfEdgeMesh);

	auto vertices = toolbox.vertices(face);
	auto numTriangles = (viscas::half_edge_mesh::index_type) vertices.size() - 2;

	auto point0 = toolbox.limitPointOrPoint(vertices[0]);
	for (viscas::half_edge_mesh::index_type i = 0; i < numTriangles; i++)
	{
		result.push_back(point0);
		result.push_back(toolbox.limitPointOrPoint(vertices[i + 1]));
		result.push_back(toolbox.limitPointOrPoint(vertices[i + 2]));
	}

	return result;
}

void OpenGLWidget::intersectFaces(const QVector3D &origin, const QVector3D &direction, bool append)
{
	viscas::topology_toolbox toolbox(halfEdgeMesh);

	auto index = viscas::topology_toolbox::invalid_face();
	auto minimum = std::numeric_limits<float>::max();

	for (auto face : toolbox.faces())
	{
		// The upcoming algorithm is only working with triangles... so triangulate the current face and run the algorithm.
		auto points = triangulateToPoints(halfEdgeMesh, face);

		for (int i = 0; i < points.size(); i++)
		{
			auto a = points[i];
			auto b = points[++i];
			auto c = points[++i];

			auto pNormal = QVector3D::normal(a, b, c);

			QVector3D result;
			float distance;

			if (rayPlaneIntersection(origin, direction, a, pNormal, result, distance))
			{
				// distance check
				if (distance < minimum)
				{
					// assumption: face contains 3 edges , 3 vertices
					// if the face contains more, face must be triangle and
					// each resulting face needs to be checked ( until hit)

					auto v0 = c - a;
					auto v1 = b - a;
					auto v2 = result - a;

					auto dot00 = QVector3D::dotProduct(v0, v0);
					auto dot01 = QVector3D::dotProduct(v0, v1);
					auto dot02 = QVector3D::dotProduct(v0, v2);
					auto dot11 = QVector3D::dotProduct(v1, v1);
					auto dot12 = QVector3D::dotProduct(v1, v2);

					auto invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
					auto u = (dot11 * dot02 - dot01 * dot12) * invDenom;
					auto v = (dot00 * dot12 - dot01 * dot02) * invDenom;

					if ((u >= .0f) && (v >= .0f) && ((u + v) < 1.0f))
					{
						minimum = distance;
						index = face;
					}
				}
			}
		}
	}

	if (invalid(index))
		return;

	if (append)
		selectionModel->addFaceConditional(index);
	else
		selectionModel->setFace(index);
}
