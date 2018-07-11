#pragma once

#include "HalfEdgeMesh.h"
//TODO
class Skin {
public:
	virtual HalfEdgeMesh* returnSkinObject() = 0;
protected:
	HalfEdgeMesh* skinMesh = nullptr;
};

