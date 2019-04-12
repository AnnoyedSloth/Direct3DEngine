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

	spawnActor<Actor>(D3DXVECTOR3(0, 0, 5), D3DXVECTOR3(50, 80, 100), D3DXVECTOR3(1, 1, 1));
	spawnActor<Actor>(D3DXVECTOR3(0, 10, 5), D3DXVECTOR3(90, 0, 0), D3DXVECTOR3(2, 2, 2));
	spawnActor<Actor>(D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(1, 1, 1));
	spawnActor<Terrain>(D3DXVECTOR3(3, 3, 3), D3DXVECTOR3(1, 1, 1), D3DXVECTOR3(1, 1, 1));

}

VOID World::Render(LPDIRECT3DDEVICE9 d3dDevice)
{
	time.CalculateDeltaTime();
	
	for (unsigned int a = 0; a < objs.size(); ++a)
	{
		objs[a]->Tick(time.deltaTime);
		objs[a]->Render(d3dDevice);
	}
}

// Factory function which spawn derived classes of Actor
template <class T>
Actor* World::spawnActor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale)
{
	// Check template class is derived by Actor class or not
	if (!(std::is_base_of<Actor , T>::value)) return nullptr;

	else
	{
		Actor* newActor = new T();

		newActor->setLocation(loc);
		newActor->setRotation(rot);
		newActor->setScale(scale);
		newActor->BeginPlay();

		objs.push_back(newActor);
		return newActor;
	}

}