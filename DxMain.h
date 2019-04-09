#pragma once
#include "Core.h"
#include "Camera.h"

class DxMain
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDevice;

	std::vector<Object*> objs;

	Camera* camera = NULL;

	DWORD mouseX;
	DWORD mouseY;

	HWND hWnd;
	


public:
	DxMain();
	~DxMain();

	HRESULT InitD3D(HWND hWnd);
	HRESULT InitGeometry();

	VOID SetupMatrices();
	VOID Render();


};