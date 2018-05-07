#include "ISelectableGraphic.h"
#include "IClickableGraphic.h"
#include "Design.h"
#include "ObjectMemory.h"

#pragma once
class GraphicObject :public ISelectableGraphic, public IClickableGraphic
{
public:
	virtual ~GraphicObject();

protected:

	bool isSelected = false;
	Design * design = nullptr;
	ObjectMemory * sMem = nullptr;
};