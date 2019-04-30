#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(LPDIRECT3DDEVICE9 device)
{
	d3dDevice = device;
}

void Mesh::createVIB()
{
	FbxLoadMgr* mgr = new FbxLoadMgr();
	mgr->importFbx("Dragon.fbx");
	mgr->GetData();

	//memcpy(vertices, mgr->GetData(), mgr->)

	delete mgr;
}