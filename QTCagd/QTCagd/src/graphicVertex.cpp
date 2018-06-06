#include "graphicVertex.h"
#include "SkinSphereVertex.h"

graphicVertex::graphicVertex(glm::vec4 location, graphicVertex * lastLOD, Design* design, ObjectMemory* sMem)
{
	this->design = design;
	this->sMem = sMem;
	this->location = location;
	this->lastLOD = lastLOD;
	SkinSphereVertex* skin = new SkinSphereVertex();
	this->design = new Design(skin,skin);
}

graphicVertex::~graphicVertex() {}
