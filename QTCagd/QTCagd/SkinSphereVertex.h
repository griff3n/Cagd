#pragma once
#include <Qt3DCore>
#include <Qt3DExtras>

class SkinSphereVertex 
{
public:
	SkinSphereVertex();
	virtual ~SkinSphereVertex();
	Qt3DExtras::QSphereMesh* returnSkinObject();
};

