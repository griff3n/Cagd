#pragma once
class ISelectableGraphic
{
public:
	virtual void select() = 0;
	virtual void deselect() = 0;
};

