#include "graphicVertex.h"
#include "SkinSphereVertex.h"

graphicVertex::graphicVertex(glm::vec4 location, graphicVertex * lastLOD,  ObjectMemory* sMem)
{
	this->sMem = sMem;
	this->location = location;
	this->lastLOD = lastLOD;
}

graphicVertex::~graphicVertex() {}
