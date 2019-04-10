#include<d3d9.h>
#pragma warning( disable : 4996 )
#include <strsafe.h>
#pragma warning( disable : 4996 )
#include <d3dx9math.h>
#pragma warning( disable : 4996 )
#include "DxMain.h"
#include "Camera.h"

DxMain::DxMain()
{
	d3d = NULL;
	d3dDevice = NULL;

	camera = new Camera();

	mouseX = 0;
	mouseY = 0;

	hWnd = NULL;
}

DxMain::~DxMain()
{
	if (d3d) d3d->Release();
	if (d3dDevice) d3dDevice->Release();
	if (camera) delete camera;
}

HRESULT DxMain::InitD3D(HWND* hWnd)
{
	if (NULL == (d3d = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, *hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &d3dDevice)))
	{
		return E_FAIL;
	}

	this->hWnd = *hWnd;
	d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	// Device state would normally be set here

	InitGeometry();


	return S_OK;
}

HRESULT DxMain::InitGeometry()
{
	objs.push_back(new Object(D3DXVECTOR3(1, 1, 1)));
	objs.push_back(new Object(D3DXVECTOR3(2, 2, 2)));
	objs.push_back(new Object(D3DXVECTOR3(3, 3, 3)));

	for (Object* obj : objs)
	{
		obj->LoadMesh(d3dDevice);
	}
	return S_OK;
}

VOID DxMain::SetupMatrices()
{
	// Set up world matrix
	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXMatrixLookAtLH(camera->GetViewMatrix(), &camera->position, &camera->lookAt, &camera->up);
	d3dDevice->SetTransform(D3DTS_VIEW, camera->GetViewMatrix());

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	d3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID DxMain::MouseInput()
{
	POINT pt;
	float delta = 0.001f;

	GetCursorPos(&pt);
	int dx = pt.x - mouseX;
	int dy = pt.y - mouseY;

	camera->RotateLocalX(dy * delta);
	camera->RotateLocalY(dx * delta);

	SetCursor(NULL);
	RECT rc;
	GetClientRect(hWnd, &rc);
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen(hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
	mouseX = pt.x;
	mouseY = pt.y;
}

VOID DxMain::KeyboardInput()
{
	if (GetAsyncKeyState('W')) camera->MoveLocalZ(.1f);
	if (GetAsyncKeyState('S')) camera->MoveLocalZ(-.1f);
	if (GetAsyncKeyState('A')) camera->MoveLocalX(-.1f);
	if (GetAsyncKeyState('D')) camera->MoveLocalX(.1f);
	if (GetAsyncKeyState('Q')) camera->MoveLocalY(-.1f);
	if (GetAsyncKeyState('E')) camera->MoveLocalY(.1f);
}

VOID DxMain::ProcessInput()
{
	MouseInput();
	KeyboardInput();
}

VOID DxMain::Render()
{
	if (NULL == d3dDevice)
		return;

	// Clear the backbuffer to a blue color
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	if (SUCCEEDED(d3dDevice->BeginScene()))
	{
		ProcessInput();
		SetupMatrices();

		for (Object* obj : objs)
		{
			obj->Render(d3dDevice);
		}
		
		// End the scene
		d3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}