#include "ISelectableGraphic.h"
#include "IClickableGraphic.h"
#include "Design.h"
#include "SelectionMemory.h"

#pragma once
class GraphicObject :public ISelectableGraphic, public IClickableGraphic
{
public:
	GraphicObject();
	GraphicObject(Design* design, SelectionMemory* sMem);


protected:

	bool isSelected = false;
	Design* design;
	SelectionMemory* sMem;
};