#include "GraphicObject.h"

GraphicObject::~GraphicObject()
{
		delete sMem;
}

void GraphicObject::select()
{
}

void GraphicObject::deselect()
{
}

void GraphicObject::onClick()
{
}

bool GraphicObject::getIsSelected()
{
	return isSelected;
}


ObjectMemory* GraphicObject::getSMem()
{
	return sMem;
}
