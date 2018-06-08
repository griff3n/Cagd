#include "SkinSphereVertex.h"


SkinSphereVertex::SkinSphereVertex()
{
	this->skinMesh = loadOBJreg("vertexSphere.obj");
}


SkinSphereVertex::~SkinSphereVertex()
{
}

HalfEdgeMesh* SkinSphereVertex::returnSkinObject()
{
	return skinMesh;
}
