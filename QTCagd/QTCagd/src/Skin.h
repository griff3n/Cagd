#pragma once
#include <Qt3DCore>
#include <Qt3DExtras>
//TODO
class Skin{
public:
	virtual ~Skin();
	virtual Qt3DCore::QEntity* returnSkinObject();
};

