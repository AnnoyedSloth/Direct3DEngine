#include "World.h"

World::World()
{
	
}

World::~World()
{

}

void World::initialize(LPDIRECT3DDEVICE9 d3dDevice, Camera* camera, Frustum* frustum, TimeManager* timeMgr)
{	
	this->d3dDevice = d3dDevice;
	this->camera = camera;
	this->frustum = frustum;
	time = timeMgr;

	spawnActor<Actor>(D3DXVECTOR3(-3, 0, 5), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	spawnActor<Actor>(D3DXVECTOR3(0, 0, 5), D3DXVECTOR3(90, 0, 0), D3DXVECTOR3(2, 2, 2));
	spawnActor<Actor>(D3DXVECTOR3(3, 0, 5), D3DXVECTOR3(0, 90, 0), D3DXVECTOR3(1, 1, 1));

	LPSTR tex[4] = { "tile2.tga", "lightmap.tga", "", "" };

	Terrain* terrain = new Terrain();
	terrain->create(d3dDevice, frustum, &D3DXVECTOR3(1.0f, 0.1f, 1.0f), 0.05,
		(LPSTR)"Textures/map129.bmp", tex);
	objs.push_back(terrain);

	for (unsigned int a = 0; a < objs.size(); ++a)
	{
		objs[a]->initialize(d3dDevice);
	}

	//myMesh = new Mesh(d3dDevice);
	//myMesh->createVIB();
}

void World::render()
{

	for (auto iter = objs.begin(); iter != objs.end(); ++iter)
	{
		(*iter)->render(time->deltaTime);
	}
	//myMesh->render();
	//frustum->draw(d3dDevice, camera->GetPos());
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