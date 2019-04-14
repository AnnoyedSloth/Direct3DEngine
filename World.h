#pragma once

#include "TimeManager.h"
#include "Terrain.h"
#include "Actor.h"
#include "Core.h"

class Camera;

class World
{
	TimeManager time;
	LPDIRECT3DDEVICE9 d3dDevice;

public:
	World();
	~World();

	Terrain* terrain;
	std::vector<Actor*> objs;
	
	VOID Initialize(LPDIRECT3DDEVICE9 d3dDevice);
	VOID Render();

	const TimeManager* getTime() const { return &time; }

	template <class T>
	Actor* spawnActor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale);
};