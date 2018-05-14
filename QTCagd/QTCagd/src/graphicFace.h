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
	int valence;
	halfEdge* edge = nullptr;
	graphicVertex* nextLOD = nullptr;
};