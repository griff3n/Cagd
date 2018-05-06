#pragma once

#include <glm/vec4.hpp>
#include "GraphicObject.h"

struct graphicVertex;
struct graphicFace;

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
};

struct graphicVertex :
	public GraphicObject
{
public:
	graphicVertex(glm::vec4 location, graphicVertex* lastLOD, graphicVertex* nextLOD, Design* design, SelectionMemory* sMem);
	~graphicVertex();
	float weight;
	glm::vec4 location;
	bool hasFlag = false;
	halfEdge*outgoing = nullptr;
	graphicVertex*nextLOD = nullptr;
	graphicVertex*lastLOD = nullptr;
};

struct graphicFace :
	public GraphicObject
{
public:
	graphicFace(Design* design, SelectionMemory* sMem);
	~graphicFace();
	bool hole;
	halfEdge* surrounding = nullptr;
	graphicVertex* nextLOD;
};