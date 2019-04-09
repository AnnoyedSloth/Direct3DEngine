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
}

DxMain::~DxMain()
{
	if (d3d) d3d->Release();
	if (d3dDevice) d3dDevice->Release();
	if (camera) delete camera;
}

HRESULT DxMain::InitD3D(HWND hWnd)
{
	if (NULL == (d3d = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//d3dpp.EnableAutoDepthStencil = TRUE;

	if (FAILED(d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &d3dDevice)))
	{
		return E_FAIL;
	}

	this->hWnd = hWnd;
	d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	// Device state would normally be set here
	objs.push_back(new Object());

	objs[0]->InitGeometry(d3dDevice);



	return S_OK;
}

HRESULT DxMain::InitGeometry()
{

	return S_OK;
}

VOID DxMain::SetupMatrices()
{
	// Set up world matrix
	D3DXMATRIXA16 matWorld;
	D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
	d3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the 
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);

	D3DXMATRIXA16 matView;

	if (GetAsyncKeyState('W')) camera->MoveLocalZ(10.0f);
	if (GetAsyncKeyState('S')) camera->MoveLocalZ(-10.0f);
	if (GetAsyncKeyState('A')) camera->MoveLocalX(-10.0f);
	if (GetAsyncKeyState('D')) camera->MoveLocalX(10.0f);

	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	d3dDevice->SetTransform(D3DTS_VIEW, &matView);

	//POINT pt;
	//float delta = 0.001f;

	//GetCursorPos(&pt);
	//int dx = pt.x - mouseX;
	//int dy = pt.y - mouseY;

	//camera->RotateLocalX(dy * delta);
	//camera->RotateLocalY(dx * delta);

	//D3DXMATRIXA16* matView = camera->GetViewMatrix();
	//d3dDevice->SetTransform(D3DTS_VIEW, matView);

	////SetCursor(NULL);
	//RECT rc;
	//GetClientRect(hWnd, &rc);
	//pt.x = (rc.right - rc.left) / 2;
	//pt.y = (rc.bottom - rc.top) / 2;
	//ClientToScreen(hWnd, &pt);
	//SetCursorPos(pt.x, pt.y);
	//mouseX = pt.x;
	//mouseY = pt.y;


	//D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	//d3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	d3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID DxMain::Render()
{
	if (NULL == d3dDevice)
		return;

	// Clear the backbuffer to a blue color
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);


	if (SUCCEEDED(d3dDevice->BeginScene()))
	{

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