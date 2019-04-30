#pragma once
#include "fbxsdk.h"
#include "FbxManager/FbxManager.h"
#include "Rendering/VertexData.h"
#include "Core/Core.h"

const int MAX_TEXTURES_COUNT = 4;

class Mesh
{
	LPDIRECT3DDEVICE9		d3dDevice;
	LPDIRECT3DTEXTURE9		texture[MAX_TEXTURES_COUNT];
	D3DMATERIAL9*       materials;
	LPDIRECT3DVERTEXBUFFER9 vBuffer;
	LPDIRECT3DINDEXBUFFER9	iBuffer;

	std::vector<Mesh> child;

	VertexData* vertices;

	unsigned int triCount;
	unsigned int vertexCount;
	unsigned int indexCount;
public:
	Mesh();
	Mesh(LPDIRECT3DDEVICE9 device);
	void createVIB();
	void render();
};