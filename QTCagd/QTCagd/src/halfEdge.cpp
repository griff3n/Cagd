#include "halfEdge.h"



halfEdge::halfEdge(Design * design, ObjectMemory * sMem)
{
	this->design = design;
	this->sMem = sMem;
}

halfEdge::~halfEdge()
{
	if (vert) {
		delete vert;
	}
	if (pair) {
		delete pair;
	}
	if (face) {
		delete face;
	}
	if (next) {
		delete next;
	}
}

