#include "ISelectableDesign.h"
#include "SkinSphereVertex.h"

#pragma once
class Design: public ISelectableDesign
{
public:
	Design(SkinSphereVertex* normal, SkinSphereVertex* selected);
	~Design();
	SkinSphereVertex* getActiveSkin();
	void applySelectedDesign();
	void applyStandardDesign();

protected:
	SkinSphereVertex *normal;
	SkinSphereVertex *selected;
	SkinSphereVertex *active;
};

