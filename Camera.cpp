#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

D3DXMATRIXA16* Camera::SetView(D3DXVECTOR3* pos, D3DXVECTOR3* look, D3DXVECTOR3* up)
{
	D3DXMatrixLookAtLH(&viewMat, pos, look, up);
	return &viewMat;
}

D3DXMATRIXA16* Camera::RotateLocalX(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &D3DXVECTOR3(1,0,0), angle);

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

	D3DXVec3Cross(&newDest, &up, &lookAt);

	D3DXVECTOR3 move;
	D3DXVec3Normalize(&move, &newDest);

	move *= dist;
	newPos += move;
	newDest += move;

	return SetView(&position, &newDest, &up);
}

D3DXMATRIXA16* Camera::MoveLocalY(float dist)
{
	D3DXVECTOR3 newPos = position;
	D3DXVECTOR3 newDest = up;

	D3DXVECTOR3 move;
	D3DXVec3Normalize(&move, &newDest);

	move *= dist;
	newPos += move;
	newDest += move;

	return SetView(&position, &newDest, &up);
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

	return SetView(&position, &newDest, &up);
}

