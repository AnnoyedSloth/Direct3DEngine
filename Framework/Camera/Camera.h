#pragma once
#include "Core/Core.h"
#include "Camera/Camera.h"

class Camera
{


	D3DXVECTOR3 view;
	D3DXVECTOR3 cross;

	D3DXMATRIXA16 viewMat;
	D3DXMATRIXA16 billMat;
	D3DXMATRIXA16 projMat;

public:
	Camera();
	~Camera();

	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 up;

	D3DXMATRIXA16* GetViewMatrix() { return &viewMat; }
	D3DXMATRIXA16* GetBillMatrix() { return &billMat; }

	D3DXMATRIXA16* SetView(D3DXVECTOR3* pos, D3DXVECTOR3* look, D3DXVECTOR3* up);
	void SetProj(D3DXMATRIXA16* proj) { projMat = *proj; }

	void SetPos(D3DXVECTOR3* pv) { position = *pv; }
	D3DXVECTOR3* GetPos() { return &position; }
	void SetLookAt(D3DXVECTOR3* pv) { lookAt = *pv; }
	D3DXVECTOR3* GetLookAt() { return &lookAt; }
	void SetUp(D3DXVECTOR3* pv) { up = *pv; }
	D3DXVECTOR3* GetUp() { return &up; }
	D3DXMATRIXA16* GetProj() { return &projMat; }

	void Flush() { SetView(&position, &lookAt, &up); }

	D3DXMATRIXA16* RotateLocalX(float angle);
	D3DXMATRIXA16* RotateLocalY(float angle);
	D3DXMATRIXA16* RotateLocalZ(float angle);

	D3DXMATRIXA16* MoveTo(D3DXVECTOR3* pv);
	D3DXMATRIXA16* MoveLocalX(float dist);
	D3DXMATRIXA16* MoveLocalY(float dist);
	D3DXMATRIXA16* MoveLocalZ(float dist);

};