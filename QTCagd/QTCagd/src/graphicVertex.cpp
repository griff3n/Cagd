#include "graphicVertex.h"
#include "SkinSphereVertex.h"

graphicVertex::graphicVertex(QVector4D location, graphicVertex * lastLOD)
{
	this->location = location;
	this->lastLOD = lastLOD;
}

graphicVertex::~graphicVertex() {}
