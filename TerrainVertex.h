#pragma once
#include <d3dx9math.h>

struct TERRAINVERTEX
{

	enum _FVF{FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;
};