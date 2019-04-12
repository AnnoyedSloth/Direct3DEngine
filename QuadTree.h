#pragma once
#include "Core.h"
#include "TerrainVertex.h"
#include "Frustum.h"


class QuadTree
{

	QuadTree* child[4];
	QuadTree* parent;
	QuadTree* neighbor[4];

	int center;
	int corner[4];

	bool culled;
	float radius;

	QuadTree* addChild(int cornerTL, int cornerTR, int cornerBL, int cornerBR);

	BOOL setCorners(int cornerTL, int cornerTR, int cornerBL, int cornerBR);

	BOOL subDivide();

	int generateIndex(int tris, LPVOID index, TERRAINVERTEX* heightMap, Frustum* frustum, float ratio);

	float getDistance(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
	{
		return D3DXVec3Length(&(*pv2 - *pv1));
	}

	int getLODLevel(TERRAINVERTEX* heightMap, D3DXVECTOR3* camera, float ratio)
	{
		float d = getDistance((D3DXVECTOR3*)(heightMap + center), camera);
		return max((int)(d*ratio), 1);
	}

	void allInFrustum();

	int isInFrustum(TERRAINVERTEX* heightMap, Frustum* frustum);

	void frustumCull(TERRAINVERTEX* heightMap, Frustum* frustum);

	void buildNeighborNode(QuadTree* root, TERRAINVERTEX* heightMap, int cx);

	BOOL buildQuadTree(TERRAINVERTEX* heightMap);

	QuadTree* findNode(TERRAINVERTEX* heightMap, int _0, int _1, int _2, int _3);

	int getNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);
	
public:

	enum CornerType{ TL, TR, BL, BR};
	enum {E_UP, E_DN, E_LT, E_RT};

	enum QuadLocation {
		FRUSTUM_OUT = 0,
		FRUSTUM_PARTIALLY_IN = 1,
		FRUSTUM_COMPLETELY_IN = 2,
		FRUSTUM_UNKNOWN = -1
	};

	QuadTree(int cx, int cy);
	QuadTree(QuadTree* parent);

	~QuadTree();

	void destroy();

	BOOL isCulled() const { return culled; }
	BOOL isVisible(TERRAINVERTEX* heightMap, D3DXVECTOR3* camera, float ratio)
	{
		return ((corner[TR] - corner[TL]) <= getLODLevel(heightMap, camera, ratio));
	}

	BOOL build(TERRAINVERTEX* heightMap);

	void getCorner(int& _0, int& _1, int& _2, int& _3)
	{
		_0 = corner[0]; _1 = corner[1];
		_2 = corner[2], _3 = corner[3];
	}

	int generateIndex(LPVOID index, TERRAINVERTEX* heightMap, Frustum* frustum, float ratio);

};