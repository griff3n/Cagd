using namespace std;

#pragma once
class ObjectMemory
{
public:
	ObjectMemory();
	~ObjectMemory();
	void subscribeSelected();
	void unsubscribeSelected();
	void registerNew();
	void deleteOld();
	void undo();
	void redo();

protected:
	//TODO hier slectionliste einbauen
	//TODO Vertex,HE&Face List einbauen

	//TODO undo-Stack
	//TODO redo-stack (die pops vom undo stack)

};



