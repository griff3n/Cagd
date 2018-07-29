#pragma once

#include "HalfEdgeMesh.h"

class MeshTester
{
public:
	MeshTester();
	~MeshTester();
	QString * testMesh(HalfEdgeMesh * mesh);
	bool testHalfEdge(HalfEdgeMesh * mesh);
	bool testFace(HalfEdgeMesh * mesh);
	bool testVertex(HalfEdgeMesh * mesh);
	QString output = "";
};

