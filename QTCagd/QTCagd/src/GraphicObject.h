#pragma once


class GraphicObject
{
public:
	virtual ~GraphicObject();
	void select();
	void deselect();
	void onClick();
	bool getIsSelected();
	void setSelected(bool selected);
	int LOD = 0;

protected:

	bool isSelected = false;
};