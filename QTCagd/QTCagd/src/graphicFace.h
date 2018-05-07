#pragma once

#include "GraphicObject.h"
#include "halfEdge.h"
#include "graphicVertex.h"

struct graphicFace :
	public GraphicObject
{
public:
	graphicFace(Design* design = nullptr, ObjectMemory* sMem = nullptr);
	~graphicFace();
	bool hole;
	halfEdge* surrounding = nullptr;
	graphicVertex* nextLOD;
};