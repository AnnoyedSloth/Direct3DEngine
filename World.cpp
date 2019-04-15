#include "World.h"

World::World()
{
	
}

World::~World()
{
	if (terrain) delete terrain;
}

VOID World::initialize(LPDIRECT3DDEVICE9 d3dDevice, Camera* camera, Frustum* frustum)
{	
	this->d3dDevice = d3dDevice;
	this->camera = camera;
	this->frustum = frustum;

	camera->SetView(&camera->position, &camera->lookAt, &camera->up);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
	frustum->make(&matProj);

	spawnActor<Actor>(D3DXVECTOR3(0, 0, 5), D3DXVECTOR3(50, 80, 100), D3DXVECTOR3(1, 1, 1));
	spawnActor<Actor>(D3DXVECTOR3(0, 0, 5), D3DXVECTOR3(90, 0, 0), D3DXVECTOR3(2, 2, 2));
	spawnActor<Actor>(D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(1, 1, 1));
	terrain = new Terrain();
	terrain->create(d3dDevice, frustum, &D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1,
		(LPSTR)"map.bmp", (LPSTR*)"Textures/grass.jpg");

}

VOID World::render()
{
	time.calculateDeltaTime();

	frustum->draw(d3dDevice);
	for (unsigned int a = 0; a < objs.size(); ++a)
	{
		objs[a]->tick(time.deltaTime);
		objs[a]->render();
	}
	//if (terrain) terrain->render();
}

// Factory function which spawn derived classes of Actor
template <class T>
Actor* World::spawnActor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale)
{
	// Check whether template class is derived by Actor class or not
	if (!(std::is_base_of<Actor , T>::value)) return nullptr;

	else
	{
		Actor* newActor = new T();

		newActor->setLocation(loc);
		newActor->setRotation(rot);
		newActor->setScale(scale);
		newActor->beginPlay();

		objs.push_back(newActor);
		return newActor;
	}

}