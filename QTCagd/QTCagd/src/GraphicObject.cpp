#include "GraphicObject.h"

GraphicObject::~GraphicObject()
{
	design->~Design();
	sMem->~ObjectMemory();
}
