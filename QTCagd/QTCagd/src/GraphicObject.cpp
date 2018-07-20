#include "GraphicObject.h"

GraphicObject::~GraphicObject()
{
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

void GraphicObject::setSelected(bool selected)
{
	isSelected = selected;
}
