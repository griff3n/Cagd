#pragma once

#include <vector>
#include <QMatrix4x4>
#include "halfEdge.h"
#include "graphicVertex.h"
#include "graphicFace.h"

class HalfEdgeMesh {
public:
	HalfEdgeMesh();
	~HalfEdgeMesh();

public:
	std::vector<graphicVertex*> vertices;
	std::vector<halfEdge*> halfEdges;
	std::vector<graphicFace*> faces;
	QMatrix4x4 model;
	float scale = 1.0f;
	HalfEdgeMesh * nextLOD = nullptr;
	HalfEdgeMesh * lastLOD = nullptr;
	int LOD = 0;
};