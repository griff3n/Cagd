#include "SkinSphereVertex.h"


SkinSphereVertex::SkinSphereVertex()
{
	this->skinMesh = loadOBJreg("ObjectFiles/vertexSphere.obj");
}


SkinSphereVertex::~SkinSphereVertex()
{
}

HalfEdgeMesh* SkinSphereVertex::returnSkinObject()
{
	return skinMesh;
}
