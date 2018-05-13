#include "Design.h"



Design::Design(Skin* normal, Skin* selected)
{
	this->normal = normal;
	this->selected = selected;
	active = normal;
}

Design::~Design()
{
	if (normal) {
		delete normal;
	}
	if (selected && selected != normal) {
		delete selected;
	}
	if (active && active != normal && active != selected) {
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
