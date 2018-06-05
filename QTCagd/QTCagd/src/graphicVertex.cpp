#include "graphicVertex.h"

graphicVertex::graphicVertex(glm::vec4 location, graphicVertex * lastLOD, Design* design, ObjectMemory* sMem)
{
	this->design = design;
	this->sMem = sMem;
	this->location = location;
	this->lastLOD = lastLOD;
}

graphicVertex::~graphicVertex() {}
