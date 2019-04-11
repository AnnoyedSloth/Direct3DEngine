#include "World.h"

World::World()
{
	
}

World::~World()
{
	if (terrain) delete terrain;
}

VOID World::Initialize()
{	
	objs.push_back(new Actor(D3DXVECTOR3(1, 1, 1)));
	objs.push_back(new Actor(D3DXVECTOR3(2, 2, 2)));
	objs.push_back(new Actor(D3DXVECTOR3(3, 3, 3)));

	for (Actor* actor : objs)
	{
		actor->BeginPlay();
	}
}

VOID World::Render(LPDIRECT3DDEVICE9 d3dDevice)
{
	using namespace std;
	time.CalculateDeltaTime();
	
	for (unsigned int a = 0; a < objs.size(); ++a)
	{
		objs[a]->Render(d3dDevice);
		objs[a]->Tick(time.deltaTime);
	}
}

