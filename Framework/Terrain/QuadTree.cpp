#include "QuadTree.h"

BOOL IsInRect(RECT* rc, POINT pt)
{
	if ((rc->left <= pt.x) && (pt.x <= rc->right) &&
		(rc->bottom <= pt.y) && (pt.y <= rc->top))
		return TRUE;

	return FALSE;
}

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
	// Set parameter as parent
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
	if (child) delete[] child;
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

int QuadTree::generateTriIndex(int tris, LPVOID index, TERRAINVERTEX* heightMap, Frustum* frustum, float ratio)
{
	if (culled) 
	{
		culled == FALSE;
		return tris;
	}

	LPDWORD p = ((LPDWORD)index) + tris * 3;

	if (isVisible(heightMap, frustum->getPos(), ratio))
	{
		// In case of leaf node, more subdividing is unfeasible
		if (corner[TR] - corner[TL] <= 1)
		{
			*p++ = corner[0];
			*p++ = corner[1];
			*p++ = corner[2];
			tris++;
			*p++ = corner[2];
			*p++ = corner[1];
			*p++ = corner[3];
			tris++;
			return tris;
		}

		BOOL b[4] = { 0, 0, 0, 0 };

		if (neighbor[E_UP]) b[E_UP] = neighbor[E_UP]->isVisible(heightMap, frustum->getPos(), ratio);
		if (neighbor[E_DN]) b[E_DN] = neighbor[E_DN]->isVisible(heightMap, frustum->getPos(), ratio);
		if (neighbor[E_LT]) b[E_LT] = neighbor[E_LT]->isVisible(heightMap, frustum->getPos(), ratio);
		if (neighbor[E_RT]) b[E_RT] = neighbor[E_RT]->isVisible(heightMap, frustum->getPos(), ratio);

		if (b[E_UP] && b[E_DN] && b[E_LT] && b[E_RT])
		{
			*p++ = corner[0];
			*p++ = corner[1];
			*p++ = corner[2];
			tris++;
			*p++ = corner[2];
			*p++ = corner[1];
			*p++ = corner[3];
			tris++;
			return tris;
		}

		int		n;

		if (!b[E_UP]) // ��� �κк����� �ʿ��Ѱ�?
		{
			n = (corner[TL] + corner[TR]) / 2;
			*p++ = center; *p++ = corner[TL]; *p++ = n; tris++;
			*p++ = center; *p++ = n; *p++ = corner[TR]; tris++;
		}
		else	// ��� �κк����� �ʿ���� ���
		{
			*p++ = center; *p++ = corner[TL]; *p++ = corner[TR]; tris++;
		}

		if (!b[E_DN]) // �ϴ� �κк����� �ʿ��Ѱ�?
		{
			n = (corner[BL] + corner[BR]) / 2;
			*p++ = center; *p++ = corner[BR]; *p++ = n; tris++;
			*p++ = center; *p++ = n; *p++ = corner[BL]; tris++;
		}
		else	// �ϴ� �κк����� �ʿ���� ���
		{
			*p++ = center; *p++ = corner[BR]; *p++ = corner[BL]; tris++;
		}

		if (!b[E_LT]) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = (corner[TL] + corner[BL]) / 2;
			*p++ = center; *p++ = corner[BL]; *p++ = n; tris++;
			*p++ = center; *p++ = n; *p++ = corner[TL]; tris++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			*p++ = center; *p++ = corner[BL]; *p++ = corner[TL]; tris++;
		}

		if (!b[E_RT]) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = (corner[TR] + corner[BR]) / 2;
			*p++ = center; *p++ = corner[TR]; *p++ = n; tris++;
			*p++ = center; *p++ = n; *p++ = corner[BR]; tris++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{
			*p++ = center; *p++ = corner[TR]; *p++ = corner[BR]; tris++;
		}

		return tris;	// �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!
	}

	// �ڽ� ���� �˻�
	if (child[TL]) tris = child[TL]->generateTriIndex(tris, index, heightMap, frustum, ratio);
	if (child[TR]) tris = child[TR]->generateTriIndex(tris, index, heightMap, frustum, ratio);
	if (child[BL]) tris = child[BL]->generateTriIndex(tris, index, heightMap, frustum, ratio);
	if (child[BR]) tris = child[BR]->generateTriIndex(tris, index, heightMap, frustum, ratio);

	return tris;

}

int QuadTree::isInFrustum(TERRAINVERTEX* heightMap, Frustum* frustum)
{
	BOOL	inSphere;

	// ��豸�ȿ� �ִ°�?
	inSphere = frustum->isInSphere((D3DXVECTOR3*)(heightMap + center), radius);
	// ��豸 �ȿ� ������ �������� �������� �׽�Ʈ�� �ʿ�����Ƿ� ����
	if (!inSphere) return FRUSTUM_OUT;

	// ����Ʈ���� 4���� ��� �������� �׽�Ʈ
	for (int a = 0; a < 4; ++a)
	{
		if (!frustum->isIn((D3DXVECTOR3*)(heightMap + corner[a]))) return FRUSTUM_PARTIALLY_IN;
	}

	return FRUSTUM_COMPLETELY_IN;

}

void	QuadTree::allInFrustum()
{
	culled = FALSE;
	if (!child[0]) return;
	for(int a=0; a<4; ++a) 	child[a]->allInFrustum();

}

void QuadTree::frustumCull(TERRAINVERTEX* heightMap, Frustum* frustum)
{
	int ret;

	culled = FALSE;
	ret = isInFrustum(heightMap, frustum);
	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN:	// �������ҿ� ��������, ������� �˻� �ʿ����
		allInFrustum();
		return;
	case FRUSTUM_PARTIALLY_IN:		// �������ҿ� �Ϻ�����, ������� �˻� �ʿ���
		culled = FALSE;
		break;
	case FRUSTUM_OUT:				// �������ҿ��� �������, ������� �˻� �ʿ����
		culled = TRUE;
		return;
	}

	for (int a = 0; a < 4; ++a)
	{
		if (child[a]) child[a]->frustumCull(heightMap, frustum);
	}
}

int	QuadTree::getNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3)
{
	int		n, _a, _b, _c, _d, gap;
	_a = _0;
	_b = _1;
	_c = _2;
	_d = _3;
	gap = _b - _a;	// ���� ����� �¿�����

	switch (ed)
	{
	case E_UP:	// ���� ���� �̿������ �ε���
		_0 = _a - cx * gap;
		_1 = _b - cx * gap;
		_2 = _a;
		_3 = _b;
		break;
	case E_DN:	// �Ʒ� ���� �̿������ �ε���
		_0 = _c;
		_1 = _d;
		_2 = _c + cx * gap;
		_3 = _d + cx * gap;
		break;
	case E_LT:	// ���� ���� �̿������ �ε���
		_0 = _a - gap;
		_1 = _a;
		_2 = _c - gap;
		_3 = _c;
		break;
	case E_RT:	// ���� ���� �̿������ �ε���
		_0 = _b;
		_1 = _b + gap;
		_2 = _d;
		_3 = _d + gap;
		if ((_0 / cx) != (_a / cx)) return -1;
		if ((_1 / cx) != (_b / cx)) return -1;
		break;
	}

	n = (_0 + _1 + _2 + _3) / 4;	// ��� �ε���
	if (!IS_IN_RANGE(n, 0, cx * cx - 1)) return -1;

	return n;
}

QuadTree* QuadTree::findNode(TERRAINVERTEX* pHeightMap, int _0, int _1, int _2, int _3)
{
	QuadTree*	p = NULL;
	// ��ġ�ϴ� ����� ��尪�� ����
	if ((corner[0] == _0) && (corner[1] == _1) && (corner[2] == _2) && (corner[3] == _3))
		return this;

	// �ڽ� ��尡 �ְ�?
	if (child[0])
	{
		RECT	rc;
		POINT	pt;
		int n = (_0 + _1 + _2 + _3) / 4;

		// ���� �ʻ󿡼��� ��ġ
		pt.x = (int)pHeightMap[n].p.x;
		pt.y = (int)pHeightMap[n].p.z;

		// 4���� �ڳʰ��� �������� �ڽĳ���� �� ���������� ��´�.
		SetRect(&rc, (int)pHeightMap[child[0]->corner[TL]].p.x,
			(int)pHeightMap[child[0]->corner[TL]].p.z,
			(int)pHeightMap[child[0]->corner[BR]].p.x,
			(int)pHeightMap[child[0]->corner[BR]].p.z);
		// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
		if (IsInRect(&rc, pt))
			return child[0]->findNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[child[1]->corner[TL]].p.x,
			(int)pHeightMap[child[1]->corner[TL]].p.z,
			(int)pHeightMap[child[1]->corner[BR]].p.x,
			(int)pHeightMap[child[1]->corner[BR]].p.z);
		if (IsInRect(&rc, pt))
			return child[1]->findNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[child[2]->corner[TL]].p.x,
			(int)pHeightMap[child[2]->corner[TL]].p.z,
			(int)pHeightMap[child[2]->corner[BR]].p.x,
			(int)pHeightMap[child[2]->corner[BR]].p.z);
		if (IsInRect(&rc, pt))
			return child[2]->findNode(pHeightMap, _0, _1, _2, _3);

		SetRect(&rc, (int)pHeightMap[child[3]->corner[TL]].p.x,
			(int)pHeightMap[child[3]->corner[TL]].p.z,
			(int)pHeightMap[child[3]->corner[BR]].p.x,
			(int)pHeightMap[child[3]->corner[BR]].p.z);
		if (IsInRect(&rc, pt))
			return child[3]->findNode(pHeightMap, _0, _1, _2, _3);
	}

	return NULL;
}

void QuadTree::buildNeighborNode(QuadTree* pRoot, TERRAINVERTEX* pHeightMap, int cx)
{
	int				n;
	int				_0, _1, _2, _3;

	for (int i = 0; i < 4; i++)
	{
		_0 = corner[0];
		_1 = corner[1];
		_2 = corner[2];
		_3 = corner[3];
		// �̿������ 4�� �ڳʰ��� ��´�.
		n = getNodeIndex(i, cx, _0, _1, _2, _3);
		// �ڳʰ����� �̿������ �����͸� ���´�.
		if (n >= 0) neighbor[i] = pRoot->findNode(pHeightMap, _0, _1, _2, _3);
	}

	// �ڽĳ��� ���ȣ��
	if (child[0])
	{
		child[0]->buildNeighborNode(pRoot, pHeightMap, cx);
		child[1]->buildNeighborNode(pRoot, pHeightMap, cx);
		child[2]->buildNeighborNode(pRoot, pHeightMap, cx);
		child[3]->buildNeighborNode(pRoot, pHeightMap, cx);
	}
}

// ����Ʈ���� �����.(Build()�Լ����� �Ҹ���)
BOOL QuadTree::buildQuadTree(TERRAINVERTEX* pHeightMap)
{
	if (subDivide())
	{
		// ������ܰ�, ���� �ϴ��� �Ÿ��� ���Ѵ�.
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + corner[TL])) -
			*((D3DXVECTOR3*)(pHeightMap + corner[BR]));
		// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 
		// 2�� ������ �������� ���Ѵ�.
		radius = D3DXVec3Length(&v) / 2.0f;
		child[TL]->buildQuadTree(pHeightMap);
		child[TR]->buildQuadTree(pHeightMap);
		child[BL]->buildQuadTree(pHeightMap);
		child[BR]->buildQuadTree(pHeightMap);
	}
	return TRUE;
}


// QuadTree�� �����Ѵ�.
BOOL QuadTree::build(TERRAINVERTEX* pHeightMap)
{
	// ����Ʈ�� ����
	buildQuadTree(pHeightMap);
	// �̿���� ����
	buildNeighborNode(this, pHeightMap, corner[TR] + 1);
	return TRUE;
}

//	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ�Ѵ�.
int	QuadTree::generateIndex(LPVOID pIndex, TERRAINVERTEX* pHeightMap, Frustum* pFrustum, float fLODRatio)
{
	// ���� �������� �ø��� �ؼ� �ø��� ������ �����Ѵ�.
	frustumCull(pHeightMap, pFrustum);
	// ����� �������� �ε����� �����ѵ�, �������� ������ �����Ѵ�.
	return generateTriIndex(0, pIndex, pHeightMap, pFrustum, fLODRatio);
}