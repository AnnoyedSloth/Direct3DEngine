#pragma once
#include "Object.h"
#include "Core/Core.h"


class Actor : public Object
{

	LPD3DXMESH          mesh;
	D3DMATERIAL9*       materials;
	LPDIRECT3DTEXTURE9* textures;
	DWORD               numMaterials;

protected:
	bool isTickRunning;

public:
	Actor();
	Actor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale);

	virtual ~Actor();

	virtual void beginPlay();
	virtual void tick(float deltaTime);

	virtual void render(float deltaTime);

	virtual void initialize(LPDIRECT3DDEVICE9 d3dDevice);
	HRESULT loadMesh(LPDIRECT3DDEVICE9 d3dDevice);

};