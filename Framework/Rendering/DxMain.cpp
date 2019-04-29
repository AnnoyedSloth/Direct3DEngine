#include <d3d9.h>
#pragma warning( disable : 4996 )
#include <strsafe.h>
#pragma warning( disable : 4996 )
#include <d3dx9math.h>
#pragma warning( disable : 4996 )
#include "DxMain.h"
#include "Camera/Camera.h"

DxMain::DxMain()
{
	d3d = NULL;
	d3dDevice = NULL;

	camera = new Camera();

	mouseX = 0;
	mouseY = 0;

	hWnd = NULL;

	isWireFrame = false;

	world = new World();
}

DxMain::~DxMain()
{
	if (d3d) d3d->Release();
	if (d3dDevice) d3dDevice->Release();
	if (camera) delete camera;
	//if (logMgr) delete logMgr;
}

HRESULT DxMain::initD3D(HWND* hWnd)
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

	//InitGeometry();


	return S_OK;
}

HRESULT DxMain::initialize()
{
	frustum = new Frustum(camera);

	//logMgr = new LogMgr(ZF_LOG_TARGET_WINDOW);

	time = new TimeManager();

	world->initialize(d3dDevice, camera, frustum, time);

	for (unsigned int a = 0; a < world->objs.size(); ++a)
	{
		world->objs[a]->loadMesh(d3dDevice);
	}

	D3DXMATRIXA16 matProj;

	// Setup projection transform
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
	d3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	camera->SetProj(&matProj);

	return S_OK;
}

VOID DxMain::setupFrustum()
{
	D3DXMATRIXA16 m;
	D3DXMATRIXA16 *view;
	view = camera->GetViewMatrix();

	D3DXMATRIXA16 frustumMatProj;
	D3DXMatrixPerspectiveFovLH(&frustumMatProj, D3DX_PI / 4, 1.0f, 1.0f, 500.0f);

	m = *view *  frustumMatProj;

	frustum->make(&m);
}

// Calculate the moving of mouse pointer using viewport axis
VOID DxMain::mouseInput()
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

	// Setup view transform
	//D3DXMatrixLookAtLH(camera->GetViewMatrix(), &camera->position, &camera->lookAt, &camera->up);
	d3dDevice->SetTransform(D3DTS_VIEW, camera->GetViewMatrix());
}

// Process keyboard inputs
VOID DxMain::keyboardInput()
{
	if (GetAsyncKeyState('W')) camera->MoveLocalZ(10.0f * time->deltaTime);
	if (GetAsyncKeyState('S')) camera->MoveLocalZ(-10.0f * time->deltaTime);
	if (GetAsyncKeyState('A')) camera->MoveLocalX(-10.0f * time->deltaTime);
	if (GetAsyncKeyState('D')) camera->MoveLocalX(10.0f * time->deltaTime);
	if (GetAsyncKeyState('Q')) camera->MoveLocalY(-10.0f * time->deltaTime);
	if (GetAsyncKeyState('E')) camera->MoveLocalY(10.0f * time->deltaTime);
	//D3DXMatrixLookAtLH(camera->GetViewMatrix(), &camera->position, &camera->lookAt, &camera->up);
	d3dDevice->SetTransform(D3DTS_VIEW, camera->GetViewMatrix());
}

// Process keyboard & mouse inputs
VOID DxMain::processInput()
{
	mouseInput();
	keyboardInput();
}

VOID DxMain::render()
{
	if (NULL == d3dDevice)
		return;

	// Clear backbuffer as white colour and clear ZBuffer
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	d3dDevice->SetRenderState(D3DRS_FILLMODE, isWireFrame ? D3DFILL_WIREFRAME : D3DFILL_SOLID);

	if (SUCCEEDED(d3dDevice->BeginScene()))
	{
		// Calculate time between before frame and current frame
		time->calculateDeltaTime();

		// Keyboard & Mouse input process
		processInput();

		// Setup frustum matrix
		setupFrustum();

		// Local & World & Camera transformation
		world->render();

		// End the scene
		d3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	d3dDevice->Present(NULL, NULL, NULL, NULL);
}