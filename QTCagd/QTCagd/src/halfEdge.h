#pragma once

#include "GraphicObject.h"
struct graphicVertex;
struct graphicFace;
#include "graphicVertex.h"
#include "graphicFace.h"

struct halfEdge :
	public GraphicObject
{
public:
	halfEdge(Design* design = nullptr, ObjectMemory* sMem = nullptr);
	~halfEdge();
	graphicVertex* vert; // start-vertex of the half-edge
	halfEdge* pair; // oppositely oriented adjacent half-edge 
	graphicFace* face; // face the half-edge borders
	halfEdge* next; // next half-edge around the face
};
