#pragma once

#include "ISelectableGraphic.h"
#include "IClickableGraphic.h"

class GraphicObject :public ISelectableGraphic, public IClickableGraphic
{
public:
	virtual ~GraphicObject();
	void select();
	void deselect();
	void onClick();
	bool selected = false;
	int LOD = 0;

protected:

};