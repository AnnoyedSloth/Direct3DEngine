#pragma once
#include "Camera.h"
#include "Core.h"
class Frustum
{
	D3DXVECTOR3 vertex[8];
	D3DXVECTOR3 pos;
	D3DXPLANE plane[6];

public:
	Frustum();
	~Frustum();

	//BOOL make(D3DXMATRIXA16* matViewProj);

	//BOOL isIn(D3DXVECTOR3* pv);

	//BOOL isInSphere(D3DXVECTOR3* pv, float radius);

	//BOOL draw(LPDIRECT3DDEVICE9 d3dDevice);

	//D3DXVECTOR3* getPos() { return &pos; }


};

