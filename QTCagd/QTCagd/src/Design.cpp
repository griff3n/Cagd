#include "Design.h"



Design::Design(Skin* normal, Skin* selected)
{
	this->normal = normal;
	this->selected = selected;
	active = normal;
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
