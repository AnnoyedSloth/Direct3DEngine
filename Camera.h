#pragma once
#include "Core.h"

class Camera
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 up;

	D3DXVECTOR3 view;
	D3DXVECTOR3 cross;
};