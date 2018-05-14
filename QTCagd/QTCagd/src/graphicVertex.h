#pragma once

#include <glm/vec4.hpp>
#include "GraphicObject.h"
struct halfEdge;
#include "halfEdge.h"

struct graphicVertex :
	public GraphicObject
{
public:
	graphicVertex(glm::vec4 location);
	graphicVertex(glm::vec4 location, graphicVertex* lastLOD, graphicVertex* nextLOD, Design* design = nullptr, ObjectMemory* sMem = nullptr);
	~graphicVertex();
	float weight;
	glm::vec4 location;
	bool hasFlag = false;
	halfEdge* outgoing = nullptr;
	graphicVertex* nextLOD = nullptr;
	graphicVertex* lastLOD = nullptr;
};