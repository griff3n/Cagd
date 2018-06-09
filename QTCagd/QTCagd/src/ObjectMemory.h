#pragma once
using namespace std;

#include "GraphicObject.h"
#include "graphicFace.h"
#include "graphicVertex.h"
#include "halfEdge.h"
#include "HalfEdgeMesh.h"
#include <vector>

class ObjectMemory
{
public:
	ObjectMemory();
	~ObjectMemory();
	void subscribeSelected();
	void unsubscribeSelected();
	void registerNew(GraphicObject* newObject, string type);
	void deleteOld();
	void addToUndo(GraphicObject[], QMatrix4x4);
	void undo();
	void redo();

protected:
	//TODO hier slectionliste einbauen
	//TODO Vertex,HE&Face List einbauen
	vector<graphicVertex*> vertexList;
	vector<halfEdge*> edgeList;
	vector<graphicFace*> faceList;
	vector<HalfEdgeMesh*> meshList;
	vector<GraphicObject* , QMatrix4x4> undoStack;


	//TODO undo-Stack
	//TODO redo-stack (die pops vom undo stack)

};



