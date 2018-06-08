#pragma once

#include "HalfEdgeMesh.h"
//TODO
class Skin{
public:
	virtual ~Skin();
	virtual HalfEdgeMesh* returnSkinObject();
protected:
	HalfEdgeMesh* skinMesh;
};

