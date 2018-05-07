#include "ObjectMemory.h"



ObjectMemory::ObjectMemory()
{
}

ObjectMemory::~ObjectMemory()
{
}

void ObjectMemory::subscribeSelected()
{
	//Subscribe as selected
}

void ObjectMemory::unsubscribeSelected()
{
	//unsub as selected
}

void ObjectMemory::registerNew()
{
	//register as new GraphicObject
}

void ObjectMemory::deleteOld()
{
	//request deletion from known Objects
}

void ObjectMemory::undo()
{
	//pop from undo-Stack and use inverse Matrix, put popped on redo-stack (not the inverse!)
}

void ObjectMemory::redo()
{
	//pop from redo Stack
}
