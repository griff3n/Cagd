#include "Design.h"



Design::Design(Skin* normal, Skin* selected)
{
	this->normal = normal;
	this->selected = selected;
	active = normal;
}

Design::~Design()
{
	normal->~Skin();
	selected->~Skin();
	active->~Skin();
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
