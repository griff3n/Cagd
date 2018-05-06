#pragma once
#include "GraphicObject.h"
#include "halfEdge.h"
struct graphicFace :
	public GraphicObject
{
public:
	graphicFace(Design* design, SelectionMemory* sMem);
	~graphicFace();
	bool hole;
	halfEdge* surrounding;
	graphicVertex* nextLOD;

private:
	typedef GraphicObject super;
};

