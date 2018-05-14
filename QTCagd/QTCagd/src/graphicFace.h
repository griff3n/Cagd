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
	bool isHole = false;
	int valence = 0;
	halfEdge* edge = nullptr;
	graphicVertex* nextLOD = nullptr;
};