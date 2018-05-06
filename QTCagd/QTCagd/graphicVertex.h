#pragma once
#include <glm/vec4.hpp>
#include "GraphicObject.h"
#include "halfEdge.h"

struct graphicVertex : public GraphicObject
{
public:
	graphicVertex(glm::vec4 location, graphicVertex* lastLOD, graphicVertex* nextLOD, Design* design, SelectionMemory* sMem);
	~graphicVertex();
	float weight;
	glm::vec4 location;
	bool hasFlag = false;
	halfEdge* outgoing;
	graphicVertex *nextLOD = nullptr;
	graphicVertex *lastLOD = nullptr;

private:
	typedef GraphicObject super;
};

