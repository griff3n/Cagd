#pragma once


class GraphicObject
{
public:
	virtual ~GraphicObject();
	void select();
	void deselect();
	void onClick();
	bool selected = false;
	int LOD = 0;

protected:

};