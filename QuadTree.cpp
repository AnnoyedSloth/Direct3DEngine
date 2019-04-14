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

int QuadTree::generateIndex(int tris, LPVOID index, TERRAINVERTEX* heightMap, Frustum* frustum, float ratio)
{
	if (culled) culled == FALSE; return tris;

	LPDWORD p = ((LPDWORD)index) + tris * 3;

	if (isVisible(heightMap, frustum->getPos(), ratio))
	{
		// In case of leaf node, subdivide is unfeasible
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

		if (!b[E_UP]) // 상단 부분분할이 필요한가?
		{
			n = (corner[TL] + corner[TR]) / 2;
			*p++ = center; *p++ = corner[TL]; *p++ = n; tris++;
			*p++ = center; *p++ = n; *p++ = corner[TR]; tris++;
		}
		else	// 상단 부분분할이 필요없을 경우
		{
			*p++ = center; *p++ = corner[TL]; *p++ = corner[TR]; tris++;
		}

		if (!b[E_DN]) // 하단 부분분할이 필요한가?
		{
			n = (corner[BL] + corner[BR]) / 2;
			*p++ = center; *p++ = corner[BR]; *p++ = n; tris++;
			*p++ = center; *p++ = n; *p++ = corner[BL]; tris++;
		}
		else	// 하단 부분분할이 필요없을 경우
		{
			*p++ = center; *p++ = corner[BR]; *p++ = corner[BL]; tris++;
		}

		if (!b[E_LT]) // 좌측 부분분할이 필요한가?
		{
			n = (corner[TL] + corner[BL]) / 2;
			*p++ = center; *p++ = corner[BL]; *p++ = n; tris++;
			*p++ = center; *p++ = n; *p++ = corner[TL]; tris++;
		}
		else	// 좌측 부분분할이 필요없을 경우
		{
			*p++ = center; *p++ = corner[BL]; *p++ = corner[TL]; tris++;
		}

		if (!b[E_RT]) // 우측 부분분할이 필요한가?
		{
			n = (corner[TR] + corner[BR]) / 2;
			*p++ = center; *p++ = corner[TR]; *p++ = n; tris++;
			*p++ = center; *p++ = n; *p++ = corner[BR]; tris++;
		}
		else	// 우측 부분분할이 필요없을 경우
		{
			*p++ = center; *p++ = corner[TR]; *p++ = corner[BR]; tris++;
		}

		return tris;	// 이 노드 아래의 자식노드는 탐색할 필요없으므로 리턴!
	}

	// 자식 노드들 검색
	if (child[TL]) tris = child[TL]->generateIndex(tris, index, heightMap, frustum, ratio);
	if (child[TR]) tris = child[TR]->generateIndex(tris, index, heightMap, frustum, ratio);
	if (child[BL]) tris = child[BL]->generateIndex(tris, index, heightMap, frustum, ratio);
	if (child[BR]) tris = child[BR]->generateIndex(tris, index, heightMap, frustum, ratio);

	return tris;

}

int QuadTree::isInFrustum(TERRAINVERTEX* heightMap, Frustum* frustum)
{
	BOOL	b[4];
	BOOL	inSphere;

	// 경계구안에 있는가?
	inSphere = frustum->isInSphere((D3DXVECTOR3*)(heightMap + center), radius);
	// 경계구 안에 없으면 점단위의 프러스텀 테스트가 필요없으므로 리턴
	if (!inSphere) return FRUSTUM_OUT;

	// 쿼드트리의 4군데 경계 프러스텀 테스트
	b[0] = frustum->isIn((D3DXVECTOR3*)(heightMap + corner[0]));
	b[1] = frustum->isIn((D3DXVECTOR3*)(heightMap + corner[1]));
	b[2] = frustum->isIn((D3DXVECTOR3*)(heightMap + corner[2]));
	b[3] = frustum->isIn((D3DXVECTOR3*)(heightMap + corner[3]));

	// 4개모두 프러스텀 안에 있음
	if ((b[0] + b[1] + b[2] + b[3]) == 4) return FRUSTUM_COMPLETELY_IN;

	// 일부분이 프러스텀에 있는 경우
	return FRUSTUM_PARTIALLY_IN;
}

void	QuadTree::allInFrustum()
{
	culled = FALSE;
	if (!child[0]) return;
	child[0]->allInFrustum();
	child[1]->allInFrustum();
	child[2]->allInFrustum();
	child[3]->allInFrustum();
}

void QuadTree::frustumCull(TERRAINVERTEX* heightMap, Frustum* frustum)
{
	int ret;

	culled = FALSE;
	ret = isInFrustum(heightMap, frustum);
	switch (ret)
	{
	case FRUSTUM_COMPLETELY_IN:	// 프러스텀에 완전포함, 하위노드 검색 필요없음
		allInFrustum();
		return;
	case FRUSTUM_PARTIALLY_IN:		// 프러스텀에 일부포함, 하위노드 검색 필요함
		culled = FALSE;
		break;
	case FRUSTUM_OUT:				// 프러스텀에서 완전벗어남, 하위노드 검색 필요없음
		culled = TRUE;
		return;
	}
	if (child[0]) child[0]->frustumCull(heightMap, frustum);
	if (child[1]) child[1]->frustumCull(heightMap, frustum);
	if (child[2]) child[2]->frustumCull(heightMap, frustum);
	if (child[3]) child[3]->frustumCull(heightMap, frustum);
}

int	QuadTree::getNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3)
{
	int		n, _a, _b, _c, _d, gap;
	_a = _0;
	_b = _1;
	_c = _2;
	_d = _3;
	gap = _b - _a;	// 현재 노드의 좌우폭값

	switch (ed)
	{
	case E_UP:	// 위쪽 방향 이웃노드의 인덱스
		_0 = _a - cx * gap;
		_1 = _b - cx * gap;
		_2 = _a;
		_3 = _b;
		break;
	case E_DN:	// 아래 방향 이웃노드의 인덱스
		_0 = _c;
		_1 = _d;
		_2 = _c + cx * gap;
		_3 = _d + cx * gap;
		break;
	case E_LT:	// 좌측 방향 이웃노드의 인덱스
		_0 = _a - gap;
		_1 = _a;
		_2 = _c - gap;
		_3 = _c;
		break;
	case E_RT:	// 우측 방향 이웃노드의 인덱스
		_0 = _b;
		_1 = _b + gap;
		_2 = _d;
		_3 = _d + gap;
		if ((_0 / cx) != (_a / cx)) return -1;
		if ((_1 / cx) != (_b / cx)) return -1;
		break;
	}

	n = (_0 + _1 + _2 + _3) / 4;	// 가운데 인덱스
	if (!IS_IN_RANGE(n, 0, cx * cx - 1)) return -1;

	return n;
}