#pragma once
#include <glm/vec4.hpp>
#include "GraphicObject.h"

struct graphicVertex : public GraphicObject
{
public:
	graphicVertex(glm::vec4 location, graphicVertex* lastLOD);
	~graphicVertex();
	bool isFlagged();

protected:
	float weight;
	glm::vec4 location;
	bool hasFlag = false;
	graphicVertex *nextLOD = nullptr;
	graphicVertex *lastLOD = nullptr;
};

