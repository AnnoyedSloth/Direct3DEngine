#pragma once
#include "Core/Core.h"

class World;

class Object
{
protected:
	D3DXMATRIXA16 worldMat;
	LPDIRECT3DDEVICE9 d3dDevice;
	World* world = nullptr;

public:
	Object();
	Object(D3DXVECTOR3 &loc);
	Object(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale);
	virtual ~Object();

	D3DXVECTOR3 location;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;

	void setLocation(D3DXVECTOR3 &loc) { location = loc; }
	void setRotation(D3DXVECTOR3 &rot) { rotation = rot; }
	void setScale(D3DXVECTOR3 &scale) { this->scale = scale; }

	void setWorld(World* world) { this->world = world; }
	World* getWorld() const { return world; }

	// Pure virual functions
	virtual VOID initialize(LPDIRECT3DDEVICE9 d3dDevice) = 0;
	virtual VOID render(float deltaTime) = 0;
};