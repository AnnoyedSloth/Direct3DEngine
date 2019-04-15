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
	Frustum* frustum;

public:
	World();
	~World();

	Terrain* terrain;
	std::vector<Actor*> objs;
	Camera* camera;
	
	VOID initialize(LPDIRECT3DDEVICE9 d3dDevice, Camera* camera, Frustum* frustum);
	VOID render();

	const TimeManager* getTime() const { return &time; }

	template <class T>
	Actor* spawnActor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale);
};