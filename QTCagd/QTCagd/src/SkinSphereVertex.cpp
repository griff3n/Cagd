#include "SkinSphereVertex.h"

SkinSphereVertex::SkinSphereVertex() {
	this->skinMesh = loadOBJreg("ObjectFiles/vertexSphere.obj");
}

SkinSphereVertex::~SkinSphereVertex() {
	delete skinMesh;
}

HalfEdgeMesh* SkinSphereVertex::returnSkinObject() {
	return skinMesh;
}
