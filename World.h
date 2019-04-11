#pragma once

#include "Core.h"
#include "TimeManager.h"
#include "Terrain.h"
#include "Actor.h"

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
};