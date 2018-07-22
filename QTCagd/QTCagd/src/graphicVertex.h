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
	int valence = 0;
	halfEdge* edge = nullptr;
	graphicVertex * nextLOD = nullptr;
	GraphicObject * lastLOD = nullptr;
	QVector4D weightedLocation();
	QVector4D limitPoint;
	QVector4D getLocation(float limit);
	void normalizeLocation();
	QVector4D smoothLocation;
};