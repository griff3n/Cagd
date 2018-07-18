#pragma once

#include "HalfEdgeMesh.h"

class MeshTester
{
public:
	MeshTester();
	~MeshTester();
	void testMesh(HalfEdgeMesh * mesh);
	bool testHalfEdge(HalfEdgeMesh * mesh);
	bool testFace(HalfEdgeMesh * mesh);
	bool testVertex(HalfEdgeMesh * mesh);
};

