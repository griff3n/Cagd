#include "halfEdge.h"



halfEdge::halfEdge(Design * design, ObjectMemory * sMem)
{
	this->design = design;
	this->sMem = sMem;
}

halfEdge::~halfEdge() {}

