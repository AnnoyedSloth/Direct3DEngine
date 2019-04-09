#pragma once
#include "Core.h"

class DxMain
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDevice;

	std::vector<Object*> objs;

	DWORD mouseX;
	DWORD mouseY;

	


public:
	DxMain();
	~DxMain();

	HRESULT InitD3D(HWND hWnd);
	HRESULT InitGeometry();

	VOID SetupMatrices();
	VOID Render();


};