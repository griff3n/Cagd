#include "halfEdge.h"




graphicVertex::graphicVertex(glm::vec4 location, graphicVertex * lastLOD, graphicVertex* nextLOD, Design* design, ObjectMemory* sMem): GraphicObject(design,sMem)
{
	this->location = location;
	this->lastLOD = lastLOD;
	this->nextLOD = nextLOD;
}

graphicVertex::~graphicVertex()
{
}
