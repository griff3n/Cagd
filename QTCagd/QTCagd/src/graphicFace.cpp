#include "graphicFace.h"


graphicFace::graphicFace(Design * design, ObjectMemory * sMem)
{
	this->design = design;
	this->sMem = sMem;
}

graphicFace::~graphicFace()
{
	if (surrounding) {
		delete surrounding;
	}
	if (nextLOD) {
		delete nextLOD;
	}
}
