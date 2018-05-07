#pragma once

#include "GraphicObject.h"
#include "halfEdge.h"
#include "graphicVertex.h"

struct graphicFace :
	public GraphicObject
{
public:
	graphicFace(Design* design, ObjectMemory* sMem);
	~graphicFace();
	bool hole;
	halfEdge* surrounding = nullptr;
	graphicVertex* nextLOD;
};