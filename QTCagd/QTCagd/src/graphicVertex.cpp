#include "graphicVertex.h"
#include "SkinSphereVertex.h"

graphicVertex::graphicVertex(QVector4D location)
{
	this->location = location;
}
QVector4D graphicVertex::weightedLocation() {
	return QVector4D(location.x() * location.w(), location.y() * location.w(), location.z() * location.w(), location.w());
}

QVector4D graphicVertex::getLocation(bool limit)
{
	if (limit) return limitPoint;
	else return location;
}

void graphicVertex::normalizeLocation() {
	location /= location.w();
}

graphicVertex::~graphicVertex() {}
