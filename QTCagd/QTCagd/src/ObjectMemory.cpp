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

void ObjectMemory::registerNew(GraphicObject* newObject, string type)
{

	//register as new GraphicObject
	if (type=="graphicVertex"){
		graphicVertex* vert = dynamic_cast<graphicVertex*>(newObject);
		vertexList.push_back(vert);
	}else if (type == "halfEdge") {
		halfEdge* he = dynamic_cast<halfEdge*>(newObject);
		edgeList.push_back(he);
	}else if (type == "graphicFace") {
		graphicFace* face = dynamic_cast<graphicFace*>(newObject);
		faceList.push_back(face);
	}
}

void ObjectMemory::deleteOld()
{
	//request deletion from known Objects
}

void ObjectMemory::addToUndo(GraphicObject changedObjects[], QMatrix4x4 change)
{
	undoStack.push_back(changedObjects, change);
}

void ObjectMemory::undo()
{
	//pop from undo-Stack and use inverse Matrix, put popped on redo-stack (not the inverse!)
}

void ObjectMemory::redo()
{
	//pop from redo Stack
}
