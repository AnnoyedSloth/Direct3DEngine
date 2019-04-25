#include "Camera.h"

Camera::Camera()
{
	position = D3DXVECTOR3(0.0f, 20.0f, -30.0f);
	lookAt = D3DXVECTOR3(-40.0f, 10.0f, 0.0f);
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	SetView(&position, &lookAt, &up);
}

Camera::~Camera()
{

}

D3DXMATRIXA16* Camera::SetView(D3DXVECTOR3* pos, D3DXVECTOR3* look, D3DXVECTOR3* vup)
{
	position = *pos;
	lookAt = *look;
	up = *vup;
	D3DXVec3Normalize(&view, &(lookAt - position));
	D3DXVec3Cross(&cross, &up, &view);

	D3DXMatrixLookAtLH(&viewMat, &position, &lookAt, &up);
	D3DXMatrixInverse(&billMat, NULL, &viewMat);
	billMat._41 = 0.0f;
	billMat._42 = 0.0f;
	billMat._43 = 0.0f;

	return &viewMat;
}

D3DXMATRIXA16* Camera::RotateLocalX(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &cross, angle);

	D3DXVECTOR3 newDst;
	D3DXVec3TransformCoord(&newDst, &view, &matRot);

	newDst += position;

	return SetView(&position, &newDst, &up);
}

D3DXMATRIXA16* Camera::RotateLocalY(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &up, angle);

	D3DXVECTOR3 newDst;
	D3DXVec3TransformCoord(&newDst, &view, &matRot);

	newDst += position;

	return SetView(&position, &newDst, &up);
}

D3DXMATRIXA16* Camera::RotateLocalZ(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &D3DXVECTOR3(0, 0, 1), angle);

	D3DXVECTOR3 newDst;
	D3DXVec3TransformCoord(&newDst, &view, &matRot);

	newDst += position;

	return SetView(&position, &newDst, &up);
}

D3DXMATRIXA16* Camera::MoveLocalX(float dist)
{
	D3DXVECTOR3 newPos = position;
	D3DXVECTOR3 newDest = lookAt;

	D3DXVECTOR3 move;
	D3DXVec3Normalize(&move, &cross);

	move *= dist;
	newPos += move;
	newDest += move;

	return SetView(&newPos, &newDest, &up);
}

D3DXMATRIXA16* Camera::MoveLocalY(float dist)
{
	D3DXVECTOR3 newPos = position;
	D3DXVECTOR3 newDest = lookAt;

	D3DXVECTOR3 move;
	D3DXVec3Normalize(&move, &up);

	move *= dist;
	newPos += move;
	newDest += move;

	return SetView(&newPos, &newDest, &up);
}


D3DXMATRIXA16* Camera::MoveLocalZ(float dist)
{
	D3DXVECTOR3 newPos = position;
	D3DXVECTOR3 newDest = lookAt;

	D3DXVECTOR3 move;
	D3DXVec3Normalize(&move, &view);

	move *= dist;
	newPos += move;
	newDest += move;

	return SetView(&newPos, &newDest, &up);
}

D3DXMATRIXA16* Camera::MoveTo(D3DXVECTOR3* pv)
{
	D3DXVECTOR3 dv = *pv - position;
	position = *pv;
	lookAt += dv;
	return SetView(&position, &lookAt, &up);
}