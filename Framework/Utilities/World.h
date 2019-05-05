#pragma once

#include "Managers/TimeManager.h"
#include "Terrain/Terrain.h"
#include "Actor.h"
#include "Rendering/Mesh.h"
#include "Core/Core.h"

class Camera;

class World
{
	TimeManager* time;
	LPDIRECT3DDEVICE9 d3dDevice;
	Frustum* frustum;

public:
	World();
	~World();

	std::vector<Object*> objs;
	//Mesh* myMesh;
	Camera* camera;
	
	void initialize(LPDIRECT3DDEVICE9 d3dDevice, Camera* camera, Frustum* frustum, TimeManager* timeMgr);
	void render();

	const TimeManager* getTime() const { return time; }

	template <class T>
	Actor* spawnActor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale);
};