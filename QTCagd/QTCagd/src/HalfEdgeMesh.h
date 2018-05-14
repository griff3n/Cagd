#pragma once

#include <vector>
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
};