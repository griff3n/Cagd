#include "graphicVertex.h"
#include "SkinSphereVertex.h"

graphicVertex::graphicVertex(QVector4D location)
{
	this->location = location;
}
QVector4D graphicVertex::weightedLocation() {
	return QVector4D(location.x() * location.w(), location.y() * location.w(), location.z() * location.w(), location.w());
}

QVector4D graphicVertex::getLocation(float t)
{
	if (t == -1.0f) {
		return limitPoint;
	}
	else if(t == 0) {
		return location;
	}
	else if (t == 1) {
		return smoothLocation;
	}
	else {
		return (1 - t) * location + t * smoothLocation;
	}
}

void graphicVertex::normalizeLocation() {
	location /= location.w();
}

graphicVertex::~graphicVertex() {}
