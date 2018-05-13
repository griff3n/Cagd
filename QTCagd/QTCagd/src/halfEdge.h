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
	graphicVertex* vert = nullptr;	// start-vertex of the half-edge
	halfEdge* pair = nullptr;		// oppositely oriented adjacent half-edge 
	graphicFace* face = nullptr;	// face the half-edge borders
	halfEdge* next = nullptr;		// next half-edge around the face
};
