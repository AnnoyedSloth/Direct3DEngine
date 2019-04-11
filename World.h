#pragma once

#include "TimeManager.h"
#include "Terrain.h"
#include "Actor.h"
#include "Core.h"

class Camera;

class World
{
	Terrain* terrain;
	TimeManager time;


public:
	World();
	~World();

	std::vector<Actor*> objs;
	
	VOID Initialize();
	VOID Render(LPDIRECT3DDEVICE9 d3dDevice);

	template <class T>
	Actor* spawnActor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale);
};