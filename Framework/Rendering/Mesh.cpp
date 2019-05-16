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

	

	if(!mgr) MessageBox(NULL, "Failed to load NPCmesh", "Vertex", MB_OK);

	vertexCount = mgr->GetVertexCount();

	if(FAILED(d3dDevice->CreateVertexBuffer(vertexCount * sizeof(VertexData), 0, VertexData::FVF, D3DPOOL_DEFAULT, &vBuffer, NULL)))
	{
		vBuffer->Release();
		MessageBox(NULL, "Failed to load Vertex buffer", "Vertex", MB_OK);
	}

	VOID* pVertices;
	
	vBuffer->Lock(0, mgr->GetVertexCount() * sizeof(VertexData), (void**)&pVertices, 0);
	memcpy(pVertices, mgr->GetData(), mgr->GetVertexCount() * sizeof(VertexData));
	vBuffer->Unlock();

	//indexCount = mgr->GetIndexCount();

	//if (FAILED(d3dDevice->CreateIndexBuffer(indexCount * sizeof(MyIndex), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &iBuffer, NULL)))
	//{
	//	MessageBox(NULL, "Failed to load Index buffer", "Vertex", MB_OK);
	//}

	//VOID* pIndices;

	//if (FAILED(iBuffer->Lock(0, indexCount * sizeof(MyIndex), (void**)&pIndices, 0)))
	//{
	//	MessageBox(NULL, "Failed to lock Index buffer", "Vertex", MB_OK);
	//}
	//memcpy(pIndices, )

	//iBuffer->Unlock();

	delete mgr;
}

void Mesh::render()
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient.r = mtrl.Diffuse.r = 0.0f;
	mtrl.Ambient.g = mtrl.Diffuse.g = 0.0f;
	mtrl.Ambient.b = mtrl.Diffuse.b = 255.0f;
	mtrl.Ambient.a = mtrl.Diffuse.a = 255.0f;

	d3dDevice->SetMaterial(&mtrl);
	d3dDevice->SetStreamSource(0, vBuffer, 0, sizeof(VertexData));
	d3dDevice->SetFVF(VertexData::FVF);
	d3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertexCount);
}