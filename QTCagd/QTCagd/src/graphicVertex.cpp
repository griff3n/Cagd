#include "graphicVertex.h"




graphicVertex::graphicVertex(glm::vec4 location, graphicVertex * lastLOD, graphicVertex* nextLOD, Design* design, ObjectMemory* sMem)
{
	this->design = design;
	this->sMem = sMem;
	this->location = location;
	this->lastLOD = lastLOD;
	this->nextLOD = nextLOD;
}

graphicVertex::~graphicVertex()
{
	lastLOD->~graphicVertex();
	nextLOD->~graphicVertex();
	outgoing->~halfEdge();
}
