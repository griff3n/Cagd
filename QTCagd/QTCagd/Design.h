#include "ISelectableDesign.h"
#include "Skin.h"

#pragma once
class Design: public ISelectableDesign
{
public:
	Design(Skin* normal, Skin* selected);
	Skin* getActiveSkin();
	void applySelectedDesign();
	void applyStandardDesign();

protected:
	Skin *normal;
	Skin *selected;
	Skin *active;
};

