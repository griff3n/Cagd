#pragma once

#include <glm/vec4.hpp>
#include "GraphicObject.h"
struct halfEdge;
#include "halfEdge.h"

struct graphicVertex :
	public GraphicObject
{
public:
	graphicVertex(glm::vec4 location, graphicVertex* lastLOD = nullptr, graphicVertex* nextLOD = nullptr, Design* design = nullptr, ObjectMemory* sMem = nullptr);
	~graphicVertex();
	float weight = 0.0f;
	glm::vec4 location;
	bool hasFlag = false;
	bool selected = false;
	int valence = 0;
	halfEdge* edge = nullptr;
	graphicVertex* nextLOD = nullptr;
	graphicVertex* lastLOD = nullptr;
};