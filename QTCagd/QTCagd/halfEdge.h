#pragma once
#include "GraphicObject.h"
#include "graphicVertex.h"
#include "graphicFace.h"

struct halfEdge :
	public GraphicObject
{
public:
	halfEdge(Design* design, SelectionMemory* sMem);
	~halfEdge();
	graphicVertex* vert; // start-vertex of the half-edge
	halfEdge* pair; // oppositely oriented adjacent half-edge 
	graphicFace* face; // face the half-edge borders
	halfEdge* next; // next half-edge around the face

private:
	typedef GraphicObject super;
};

