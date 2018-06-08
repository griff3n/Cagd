#pragma once

#include "ISelectableGraphic.h"
#include "IClickableGraphic.h"
#include "ObjectMemory.h"

class GraphicObject :public ISelectableGraphic, public IClickableGraphic
{
public:
	virtual ~GraphicObject();
	void select();
	void deselect();
	void onClick();
	bool getIsSelected();
	ObjectMemory* getSMem();

protected:

	bool isSelected = false;
	ObjectMemory * sMem = nullptr;
};