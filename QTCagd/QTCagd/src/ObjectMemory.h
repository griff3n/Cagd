#pragma once
#include "GraphicObject.h"
#include "graphicFace.h"
#include "graphicVertex.h"
#include "halfEdge.h"
#include "HalfEdgeMesh.h"

using namespace std;

class ObjectMemory
{
public:
	ObjectMemory();
	~ObjectMemory();
	void subscribeSelected(GraphicObject* selected);
	void unsubscribeSelected(GraphicObject* selected);
	void registerNew(GraphicObject* newObject, string type);
	void deleteOld();
	void addToUndo(GraphicObject[], QMatrix4x4*);
	void undo();
	void redo();

protected:
	vector<graphicVertex*> vertexList;
	vector<halfEdge*> edgeList;
	vector<graphicFace*> faceList;
	vector<HalfEdgeMesh*> meshList;
	list<GraphicObject*> selectionList;
	vector<GraphicObject*> undoStackObjects;
	vector<QMatrix4x4*> undoStackMatrix;
	vector<GraphicObject*> redoStackObjects;
	vector<QMatrix4x4*> redoStackMatrix;
};



