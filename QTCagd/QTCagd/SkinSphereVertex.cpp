#include "SkinSphereVertex.h"



SkinSphereVertex::SkinSphereVertex()
{
}


SkinSphereVertex::~SkinSphereVertex()
{
}

Qt3DExtras::QSphereMesh* SkinSphereVertex::returnSkinObject()
{
	// Sphere shape data
	Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
	sphereMesh->setRings(20);
	sphereMesh->setSlices(20);
	sphereMesh->setRadius(2);

	return sphereMesh;
}
