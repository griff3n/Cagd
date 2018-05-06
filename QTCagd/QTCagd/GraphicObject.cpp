#include "GraphicObject.h"

GraphicObject::GraphicObject()
{
	design = nullptr;
	sMem = nullptr;
}

GraphicObject::GraphicObject(Design* design, SelectionMemory* sMem)
{
	this->design = design;
	this->sMem = sMem;
}
