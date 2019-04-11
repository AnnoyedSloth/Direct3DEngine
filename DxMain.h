#pragma once
#include "Core.h"
#include "Camera.h"
#include "TimeManager.h"
#include "World.h"

class DxMain
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDevice;

	World* world;

	Camera* camera = NULL;

	DWORD mouseX;
	DWORD mouseY;

	HWND hWnd;
	


public:
	DxMain();
	~DxMain();

	HRESULT InitD3D(HWND* hWnd);
	HRESULT Initialize();

	VOID SetupMatrices();
	VOID Render();
	VOID ProcessInput();
	VOID MouseInput();
	VOID KeyboardInput();


};