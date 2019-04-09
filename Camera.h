#pragma once
#include "Core.h"

class Camera
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 up;

	D3DXVECTOR3 view;
	D3DXVECTOR3 cross;

	D3DXMATRIXA16 viewMat;
	D3DXMATRIXA16 billMat;

public:
	Camera();
	~Camera();
	D3DXMATRIXA16* GetViewMatrix() { return &viewMat; }
	D3DXMATRIXA16* GetBillMatrix() { return &billMat; }

	D3DXMATRIXA16* SetView(D3DXVECTOR3* pos, D3DXVECTOR3* look, D3DXVECTOR3* up);

	void SetPos(D3DXVECTOR3* pv) { position = *pv; }
	D3DXVECTOR3* GetPos() { return &position; }
	void SetLookAt(D3DXVECTOR3* pv) { lookAt = *pv; }
	D3DXVECTOR3* GetLookAt() { return &lookAt; }
	void SetUp(D3DXVECTOR3* pv) { up = *pv; }
	D3DXVECTOR3* GetUp() { return &up; }

	void Flush() { SetView(&position, &lookAt, &up); }

	D3DXMATRIXA16* RotateLocalX(float angle);
	D3DXMATRIXA16* RotateLocalY(float angle);
	D3DXMATRIXA16* RotateLocalZ(float angle);

	//D3DXMATRIXA16* MoveTo(D3DXVECTOR3* pv);
	D3DXMATRIXA16* MoveLocalX(float dist);
	D3DXMATRIXA16* MoveLocalY(float dist);
	D3DXMATRIXA16* MoveLocalZ(float dist);

};