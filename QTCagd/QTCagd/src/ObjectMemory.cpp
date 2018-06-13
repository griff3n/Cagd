#include "ObjectMemory.h"



ObjectMemory::ObjectMemory()
{
}

ObjectMemory::~ObjectMemory()
{
}

void ObjectMemory::subscribeSelected(GraphicObject* selected)
{
	bool found = (std::find(selectionList.begin(), selectionList.end(), selected) != selectionList.end());
	if (!found) { selectionList.push_back(selected); }
}

void ObjectMemory::unsubscribeSelected(GraphicObject* deselected)
{
	bool found = (std::find(selectionList.begin(), selectionList.end(), deselected) != selectionList.end());
	if (found) { selectionList.remove(deselected); }
}

void ObjectMemory::registerNew(GraphicObject* newObject, string type)
{

	//register as new GraphicObject
	if (type == "graphicVertex") {
		graphicVertex* vert = dynamic_cast<graphicVertex*>(newObject);
		vertexList.push_back(vert);
	}
	else if (type == "halfEdge") {
		halfEdge* he = dynamic_cast<halfEdge*>(newObject);
		edgeList.push_back(he);
	}
	else if (type == "graphicFace") {
		graphicFace* face = dynamic_cast<graphicFace*>(newObject);
		faceList.push_back(face);
	}
}

void ObjectMemory::deleteOld()
{
	//request deletion from known Objects
}

void ObjectMemory::addToUndo(GraphicObject changedObjects[], QMatrix4x4* change)
{
	undoStackObjects.push_back(changedObjects);
	undoStackMatrix.push_back(change);
}

void ObjectMemory::undo()
{
	//pop from undo-Stack and use inverse Matrix, put popped on redo-stack (not the inverse!)
	GraphicObject* undoObjects = undoStackObjects.back();
	undoStackObjects.pop_back();
	QMatrix4x4* undoChange = undoStackMatrix.back();
	undoStackMatrix.pop_back();

	redoStackObjects.push_back(undoObjects);
	redoStackMatrix.push_back(undoChange);

	undoChange = undoChange->inverted;
	//undo changes here
}

void ObjectMemory::redo()
{
	//pop from redo Stack
	GraphicObject* redoObjects = redoStackObjects.back();
	redoStackObjects.pop_back();
	QMatrix4x4* redoChange = redoStackMatrix.back();
	redoStackMatrix.pop_back();

	//redo changes here
}
