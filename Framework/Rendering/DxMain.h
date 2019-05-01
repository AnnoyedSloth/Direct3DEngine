#pragma once
#include "Core/Core.h"
#include "Camera/Camera.h"
#include "Managers/TimeManager.h"
#include "Utilities/World.h"
//#include "LogManager.h"

class Frustum;

static class DxMain
{
private:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDevice;

	World* world;

	TimeManager* time;

	Camera* camera = NULL;
	Frustum* frustum = NULL;

	DWORD mouseX;
	DWORD mouseY;

	HWND hWnd;
	
	//LogMgr* logMgr;
	BOOL isWireFrame;


public:
	DxMain();
	~DxMain();

	HRESULT initD3D(HWND* hWnd);
	HRESULT initialize();

	VOID setupFrustum();
	VOID render();
	VOID processInput();
	VOID mouseInput();
	VOID keyboardInput();


};