#include "ISelectableGraphic.h"
#include "IClickableGraphic.h"
#include "Design.h"
#include "ObjectMemory.h"

#pragma once
class GraphicObject :public ISelectableGraphic, public IClickableGraphic
{
public:
	GraphicObject();
	GraphicObject(Design * design, ObjectMemory * sMem);


protected:

	bool isSelected = false;
	Design * design;
	ObjectMemory * sMem;
};