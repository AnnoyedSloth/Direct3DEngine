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
	if (child[TL]) tris = child[TL]->generateTriIndex(tris, index, heightMap, frustum, ratio);
	if (child[TR]) tris = child[TR]->generateTriIndex(tris, index, heightMap, frustum, ratio);
	if (child[BL]) tris = child[BL]->generateTriIndex(tris, index, heightMap, frustum, ratio);
	if (child[BR]) tris = child[BR]->generateTriIndex(tris, index, heightMap, frustum, ratio);

	return tris;

}

int QuadTree::isInFrustum(TERRAINVERTEX* heightMap, Frustum* frustum)
{
	BOOL	inSphere;

	// 경계구안에 있는가?
	inSphere = frustum->isInSphere((D3DXVECTOR3*)(heightMap + center), radius);
	// 경계구 안에 없으면 점단위의 프러스텀 테스트가 필요없으므로 리턴
	if (!inSphere) return FRUSTUM_OUT;

	// 쿼드트리의 4군데 경계 프러스텀 테스트
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

QuadTree* QuadTree::findNode(TERRAINVERTEX* pHeightMap, int _0, int _1, int _2, int _3)
{
	QuadTree*	p = NULL;
	// 일치하는 노드라면 노드값을 리턴
	if ((corner[0] == _0) && (corner[1] == _1) && (corner[2] == _2) && (corner[3] == _3))
		return this;

	// 자식 노드가 있가?
	if (child[0])
	{
		RECT	rc;
		POINT	pt;
		int n = (_0 + _1 + _2 + _3) / 4;

		// 현재 맵상에서의 위치
		pt.x = (int)pHeightMap[n].p.x;
		pt.y = (int)pHeightMap[n].p.z;

		// 4개의 코너값을 기준으로 자식노드의 맵 점유범위를 얻는다.
		SetRect(&rc, (int)pHeightMap[child[0]->corner[TL]].p.x,
			(int)pHeightMap[child[0]->corner[TL]].p.z,
			(int)pHeightMap[child[0]->corner[BR]].p.x,
			(int)pHeightMap[child[0]->corner[BR]].p.z);
		// pt값이 점유범위안에 있다면 자식노드로 들어간다.
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
		// 이웃노드의 4개 코너값을 얻는다.
		n = getNodeIndex(i, cx, _0, _1, _2, _3);
		// 코너값으로 이웃노드의 포인터를 얻어온다.
		if (n >= 0) neighbor[i] = pRoot->findNode(pHeightMap, _0, _1, _2, _3);
	}

	// 자식노드로 재귀호출
	if (child[0])
	{
		child[0]->buildNeighborNode(pRoot, pHeightMap, cx);
		child[1]->buildNeighborNode(pRoot, pHeightMap, cx);
		child[2]->buildNeighborNode(pRoot, pHeightMap, cx);
		child[3]->buildNeighborNode(pRoot, pHeightMap, cx);
	}
}

// 쿼드트리를 만든다.(Build()함수에서 불린다)
BOOL QuadTree::buildQuadTree(TERRAINVERTEX* pHeightMap)
{
	if (subDivide())
	{
		// 좌측상단과, 우측 하단의 거리를 구한다.
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap + corner[TL])) -
			*((D3DXVECTOR3*)(pHeightMap + corner[BR]));
		// v의 거리값이 이 노드를 감싸는 경계구의 지름이므로, 
		// 2로 나누어 반지름을 구한다.
		radius = D3DXVec3Length(&v) / 2.0f;
		child[TL]->buildQuadTree(pHeightMap);
		child[TR]->buildQuadTree(pHeightMap);
		child[BL]->buildQuadTree(pHeightMap);
		child[BR]->buildQuadTree(pHeightMap);
	}
	return TRUE;
}


// QuadTree를 구축한다.
BOOL QuadTree::build(TERRAINVERTEX* pHeightMap)
{
	// 쿼드트리 구축
	buildQuadTree(pHeightMap);
	// 이웃노드 구축
	buildNeighborNode(this, pHeightMap, corner[TR] + 1);
	return TRUE;
}

//	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환한다.
int	QuadTree::generateIndex(LPVOID pIndex, TERRAINVERTEX* pHeightMap, Frustum* pFrustum, float fLODRatio)
{
	// 먼저 프러스텀 컬링을 해서 컬링될 노드들을 배제한다.
	frustumCull(pHeightMap, pFrustum);
	// 출력할 폴리곤의 인덱스를 생성한뒤, 폴리곤의 개수를 리턴한다.
	return generateTriIndex(0, pIndex, pHeightMap, pFrustum, fLODRatio);
}