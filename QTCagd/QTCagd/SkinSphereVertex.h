#pragma once
#include <Qt3DCore>
#include <Qt3DExtras>
#include "src/Skin.h"

class SkinSphereVertex : public Skin
{
public:
	SkinSphereVertex();
	virtual ~SkinSphereVertex();
	Qt3DCore::QEntity* returnSkinObject();
};

