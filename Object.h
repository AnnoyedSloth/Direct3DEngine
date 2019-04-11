#pragma once
#include "Core.h"

class World;

class Object
{
	LPD3DXMESH          mesh;
	D3DMATERIAL9*       materials;
	LPDIRECT3DTEXTURE9* textures;
	DWORD               numMaterials;

	D3DXMATRIXA16 worldMat;

	World* world = nullptr;

public:
	D3DXVECTOR3 location;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;

	Object() {};
	Object(D3DXVECTOR3 &loc);
	Object(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale);
	virtual ~Object();

	HRESULT LoadMesh(LPDIRECT3DDEVICE9 d3dDevice);
	VOID Render(LPDIRECT3DDEVICE9 d3dDevice);

	void setLocation(D3DXVECTOR3 &loc) { location = loc; }
	void setRotation(D3DXVECTOR3 &rot) { rotation = rot; }
	void setScale(D3DXVECTOR3 &scale) { this->scale = scale; }


	void setWorld(World* world) { this->world = world; }
	World* getWorld() const { return world; }

};