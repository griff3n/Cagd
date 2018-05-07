#include "GraphicObject.h"

GraphicObject::~GraphicObject()
{
	design->~Design();
	sMem->~ObjectMemory();
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

Design* GraphicObject::getDesign()
{
	return design;
}

ObjectMemory* GraphicObject::getSMem()
{
	return sMem;
}
