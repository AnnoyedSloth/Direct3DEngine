#include "Camera/Frustum.h"



Frustum::Frustum()
{
	ZeroMemory(vertex, sizeof(vertex[0]) * 8);
	ZeroMemory(plane, sizeof(plane[0]) * 6);
}

Frustum::Frustum(Camera* camera)
{
	mainCamera = camera;
	ZeroMemory(vertex, sizeof(vertex[0]) * 8);
	ZeroMemory(plane, sizeof(plane[0]) * 6);
}


Frustum::~Frustum()
{

}

BOOL Frustum::make(D3DXMATRIXA16* matViewProj)
{
	D3DXMATRIXA16	matInv;

	// Left Up Near
	vertex[0].x = -1.0f;	vertex[0].y = -1.0f;	vertex[0].z = 0.0f;
	// Right Up Near
	vertex[1].x = 1.0f;		vertex[1].y = -1.0f;	vertex[1].z = 0.0f;
	// Right Up Far
	vertex[2].x = 1.0f;		vertex[2].y = -1.0f;	vertex[2].z = 1.0f;
	// Left Up Far
	vertex[3].x = -1.0f;	vertex[3].y = -1.0f;	vertex[3].z = 1.0f;
	// Left Down Near
	vertex[4].x = -1.0f;	vertex[4].y = 1.0f;		vertex[4].z = 0.0f;
	// Right Down Near
	vertex[5].x = 1.0f;		vertex[5].y = 1.0f;		vertex[5].z = 0.0f;
	// Right Down Far
	vertex[6].x = 1.0f;		vertex[6].y = 1.0f;		vertex[6].z = 1.0f;
	// Left Down Far
	vertex[7].x = -1.0f;	vertex[7].y = 1.0f;		vertex[7].z = 1.0f;

	D3DXMatrixInverse(&matInv, NULL, matViewProj);

	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&vertex[i], &vertex[i], &matInv);

	// Position of the camera
	pos = (vertex[0] + vertex[5]) / 2.0f;

	D3DXPlaneFromPoints(&plane[0], vertex + 4, vertex + 7, vertex + 6);	// top
	D3DXPlaneFromPoints(&plane[1], vertex, vertex + 1, vertex + 2);	// bottom
	D3DXPlaneFromPoints(&plane[2], vertex, vertex + 4, vertex + 5);	// near
	D3DXPlaneFromPoints(&plane[3], vertex + 2, vertex + 6, vertex + 7);	// far
	D3DXPlaneFromPoints(&plane[4], vertex, vertex + 3, vertex + 7);	// left
	D3DXPlaneFromPoints(&plane[5], vertex + 1, vertex + 5, vertex + 6);	//right

	return TRUE;
}

BOOL Frustum::isIn(D3DXVECTOR3* pv)
{
	float fDist;

	for( int a = 0; a < 6; ++a )
	{
		fDist = D3DXPlaneDotCoord(&plane[a], pv);
		if (fDist > PLANE_EPSILON) return FALSE;
	}

	return TRUE;
}

BOOL Frustum::isInSphere(D3DXVECTOR3* pv, float radius)
{
	float		fDist;

	for (int a = 0; a < 6; ++a)
	{
		fDist = D3DXPlaneDotCoord(&plane[3], pv);
		if (fDist > (radius + PLANE_EPSILON)) return FALSE;
	}

	return TRUE;
}

BOOL Frustum::draw(LPDIRECT3DDEVICE9 d3dDevice, const D3DXVECTOR3* cameraPos)
{

	//DWORD		index[] = { 0, 1, 2,
	//						0, 2, 3,
	//						4, 7, 6,
	//						4, 6, 5,
	//						1, 5, 6,
	//						1, 6, 2,
	//						0, 3, 7,
	//						0, 7, 4,
	//						0, 4, 5,
	//						0, 5, 1,
	//						3, 7, 6,
	//						3, 6, 2 };

	//D3DMATERIAL9 mtrl;
	//ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));

	//typedef struct tagVTX
	//{
	//	D3DXVECTOR3	p;
	//} VTX;

	//VTX		vtx[8];

	//for (int i = 0; i < 8; i++)
	//	vtx[i].p = vertex[i];

	//d3dDevice->SetFVF(D3DFVF_XYZ);
	//d3dDevice->SetStreamSource(0, NULL, 0, sizeof(VTX));
	//d3dDevice->SetTexture(0, NULL);
	//d3dDevice->SetIndices(0);

	//D3DXMATRIXA16 pos;
	//D3DXMatrixTranslation(&pos, 0, 0, 0);
	//D3DXMatrixTranslation(&pos, mainCamera->GetPos()->x, mainCamera->GetPos()->y, mainCamera->GetPos()->z);
	//d3dDevice->SetTransform(D3DTS_WORLD, &pos);
	//d3dDevice->SetTransform(D3DTS_WORLD, &pos);

	//d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	//d3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	//d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//// 파란색으로 상,하 평면을 그린다.
	//d3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	//mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	//d3dDevice->SetMaterial(&mtrl);
	//d3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX32, vtx, sizeof(vtx[0]));

	//// 녹색으로 좌,우 평면을 그린다.
	//ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	//mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	//d3dDevice->SetMaterial(&mtrl);
	//d3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 12, D3DFMT_INDEX32, vtx, sizeof(vtx[0]));

	// 붉은색으로 원,근 평면을 그린다.
	//ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	//mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	//mtrl.Diffuse.a = mtrl.Ambient.a = 0.3f;
	//d3dDevice->SetMaterial(&mtrl);
	//d3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 8, 4, index + 24, D3DFMT_INDEX32, vtx, sizeof(vtx[0]));

	//d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	return TRUE;
}

void MakePlane(D3DXPLANE* pPlane, D3DXVECTOR3* pv0, D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
{
	D3DXPlaneFromPoints(pPlane, pv0, pv1, pv2);
}