#pragma once

#include "ObjectLoader.h"
#include "Skin.h"

class SkinSphereVertex : public Skin {
public:
	SkinSphereVertex();
	virtual ~SkinSphereVertex();
	HalfEdgeMesh* returnSkinObject();
};

