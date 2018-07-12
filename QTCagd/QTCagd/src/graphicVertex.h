#pragma once

#include "GraphicObject.h"
struct halfEdge;
#include "halfEdge.h"
#include <QVector4D>

struct graphicVertex :
	public GraphicObject
{
public:
	graphicVertex(QVector4D location);
	~graphicVertex();
	float weight = 0.0f;
	QVector4D location;
	bool sharp = false;
	bool selected = false;
	int valence = 0;
	halfEdge* edge = nullptr;
	graphicVertex * nextLOD = nullptr;
	GraphicObject * lastLOD = nullptr;
};