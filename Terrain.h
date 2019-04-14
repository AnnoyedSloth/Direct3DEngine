#pragma once
#include "Frustum.h"
#include "QuadTree.h"
#include "Core.h"

const int MAX_TERRAIN_TEXTURE = 4;

class Terrain
{

	int xPxl;
	int zPxl;
	int yPxl;

	D3DXVECTOR3				scale;
	TERRAINVERTEX*			heightMap;
	LPDIRECT3DDEVICE9		d3dDevice;
	LPDIRECT3DTEXTURE9		texture[MAX_TERRAIN_TEXTURE];
	LPDIRECT3DVERTEXBUFFER9 vBuffer;
	LPDIRECT3DINDEXBUFFER9	iBuffer;
	int						triangles;
	QuadTree*				quadTree;
	float					ratio;

	HRESULT destroy();
	HRESULT loadTextures(LPSTR texName[MAX_TERRAIN_TEXTURE]);
	HRESULT buildHeightMap(LPSTR fileName);
	HRESULT buildQuadTree();
	HRESULT createVIB();
	HRESULT render();

public:
	HRESULT create(LPDIRECT3DDEVICE9 d3dDevice, D3DXVECTOR3* scale, float ratio, LPSTR fileName, LPSTR texFileName[MAX_TERRAIN_TEXTURE]);

	TERRAINVERTEX* getVertex(int x, int z) { return (heightMap + x + z * xPxl); }

	float getHeight(int x, int z) { return (getVertex(x, z))->p.y; }
	int getXPxl() { return xPxl; }
	int getZPxl() { return zPxl; }
	float getXTerrain() { return xPxl * scale.x; }
	float getYTerrain() { return yPxl * scale.y; }
	float getZTerrain() { return zPxl * scale.z; }

	HRESULT draw(Frustum* frustum);


};