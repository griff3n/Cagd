#include "graphicVertex.h"





graphicVertex::graphicVertex(glVertex4f location, graphicVertex * lastLOD)
{
}

graphicVertex::~graphicVertex()
{
}

bool graphicVertex::isFlagged()
{
	return hasFlag;
}
