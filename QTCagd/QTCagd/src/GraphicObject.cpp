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
	return selected;
}

void GraphicObject::setSelected(bool selected)
{
	this->selected = selected;
}
