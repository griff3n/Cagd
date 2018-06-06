#include "Design.h"



Design::Design(Skin* normal, Skin* selected)
{
	this->normal = normal;
	this->selected = selected;
	active = normal;
}

Design::~Design()
{
	delete normal;
	if (selected != normal) {
		delete selected;
	}
	if (active != normal && active != selected) {
		delete active;
	}
}

Skin* Design::getActiveSkin()
{
	return active;
}

void Design::applySelectedDesign()
{
	active = selected;
}

void Design::applyStandardDesign()
{
	active = normal;
}
