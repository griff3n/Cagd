#include "graphicVertex.h"
#include "SkinSphereVertex.h"

graphicVertex::graphicVertex(QVector4D location)
{
	this->location = location;
}
QVector4D graphicVertex::weightedLocation() {
	return QVector4D(location.x() * location.w(), location.y() * location.w(), location.z() * location.w(), location.w());
}

void graphicVertex::normalizeLocation() {
	location /= location.w();
}

graphicVertex::~graphicVertex() {}
