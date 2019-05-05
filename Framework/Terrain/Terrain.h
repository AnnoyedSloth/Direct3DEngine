#pragma once
#include "Camera/Frustum.h"
#include "Utilities/Object.h"
#include "QuadTree.h"
#include "ExternalLibrary/Dib.h"
#include "Core/Core.h"

const int MAX_TERRAIN_TEXTURE = 4;

class Terrain : public Object
{

	int xPxl;
	int zPxl;
	int yPxl;

	Frustum* frustum;
	D3DXVECTOR3				scale;
	TERRAINVERTEX*			heightMap;
	LPDIRECT3DDEVICE9		d3dDevice;
	LPDIRECT3DTEXTURE9		texture[MAX_TERRAIN_TEXTURE];
	D3DMATERIAL9*       materials;
	LPDIRECT3DVERTEXBUFFER9 vBuffer;
	LPDIRECT3DINDEXBUFFER9	iBuffer;
	int						triangles;
	QuadTree*				quadTree;
	float					ratioLOD;

	HRESULT destroy();
	HRESULT loadTextures(LPSTR texName[MAX_TERRAIN_TEXTURE]);
	HRESULT buildHeightMap(LPSTR fileName);
	HRESULT buildQuadTree();
	HRESULT createVIB();

public:
	Terrain();
	~Terrain();

	virtual void initialize(LPDIRECT3DDEVICE9 d3dDevice) { this->d3dDevice = d3dDevice; }
	HRESULT create(LPDIRECT3DDEVICE9 d3dDevice, Frustum* frustum, D3DXVECTOR3* scale, float ratio, LPSTR bmpName, LPSTR texName[MAX_TERRAIN_TEXTURE]);

	TERRAINVERTEX* getVertex(int x, int z) { return (heightMap + x + z * xPxl); }

	float getHeight(int x, int z) { return (getVertex(x, z))->p.y; }
	int getXPxl() { return xPxl; }
	int getZPxl() { return zPxl; }
	float getXTerrain() { return xPxl * scale.x; }
	float getYTerrain() { return yPxl * scale.y; }
	float getZTerrain() { return zPxl * scale.z; }

	virtual void render(float deltaTime);


};