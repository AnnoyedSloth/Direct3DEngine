#pragma once
#include "Core.h"

class Object
{
	LPD3DXMESH          mesh;
	D3DMATERIAL9*       materials;
	LPDIRECT3DTEXTURE9* textures;
	DWORD               numMaterials;

public:
	D3DXVECTOR3 location;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;

	Object();
	virtual ~Object();

	HRESULT InitGeometry(LPDIRECT3DDEVICE9 d3dDevice);
	VOID Render(LPDIRECT3DDEVICE9 d3dDevice);

};