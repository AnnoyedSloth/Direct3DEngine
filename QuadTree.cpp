#include "QuadTree.h"

QuadTree::QuadTree(int cx, int cy)
{
	parent = NULL;

	for (int a = 0; a < 4; ++a)
	{
		child[a] = NULL;
		neighbor[a] = NULL;
	}

	corner[TL] = 0;
	corner[TR] = cx - 1;
	corner[BL] = cx * (cy - 1);
	corner[BR] = cx * cy - 1;
	center = (corner[TL] + corner[TR] + corner[BL] + corner[BR]) / 4;
	culled = FALSE;
	radius = 0;
}

QuadTree::QuadTree(QuadTree* parent)
{
	this->parent = parent;
	center = 0;

	for (int a = 0; a < 4; ++a)
	{
		child[a] = NULL;
		neighbor[a] = NULL;
		corner[a] = 0;
	}
	culled = FALSE;
	radius = 0;
}

QuadTree::~QuadTree()
{
	destroy();
}

void QuadTree::destroy()
{
	delete [] child;
}

BOOL QuadTree::setCorners(int cornerTL, int cornerTR, int cornerBL, int cornerBR)
{
	corner[TL] = cornerTL;
	corner[TR] = cornerTR;
	corner[BL] = cornerBL;
	corner[BR] = cornerBR;
	center = (corner[TL] + corner[TR] + corner[BL] + corner[BR]) / 4;
	return TRUE;
}			 

QuadTree* QuadTree::addChild(int cornerTL, int cornerTR, int cornerBL, int cornerBR)
{
	QuadTree* newChild;
	newChild = new QuadTree(this);
	newChild->setCorners(cornerTL, cornerTR, cornerBL, cornerBR);
	return newChild;
}

BOOL QuadTree::subDivide()
{
	int topEdgeCenter;
	int bottomEdgeCenter;
	int leftEdgeCenter;
	int rightEdgeCenter;
	int centralPoint;

	topEdgeCenter = (corner[TL] + corner[TR]) / 2;
	bottomEdgeCenter = (corner[BL] + corner[BR]) / 2;
	leftEdgeCenter = (corner[TL] + corner[BL]) / 2;
	rightEdgeCenter = (corner[TR] + corner[BR]) / 2;
	centralPoint = (corner[TL] + corner[TR] + corner[BL] + corner[BR]) / 4;

	if (corner[TR] - corner[TL] <= 1) return FALSE;

	child[TL] = addChild(corner[TL], topEdgeCenter, leftEdgeCenter, centralPoint);
	child[TR] = addChild(topEdgeCenter, corner[TR], centralPoint, rightEdgeCenter);
	child[BL] = addChild(leftEdgeCenter, centralPoint, corner[BL], bottomEdgeCenter);
	child[BR] = addChild(centralPoint, rightEdgeCenter, bottomEdgeCenter, corner[BR]);


	return TRUE;
}