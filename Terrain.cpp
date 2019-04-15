#include "Terrain.h"

int	Log2(int n)
{
	for (int i = 1; i < 64; i++)
	{
		n = n >> 1;
		if (n == 1) return i;
	}

	return 1;
}

// 2^n값을 구한다
int	Pow2(int n)
{
	int val = 1;
	val = val << n;
	return val;
}

Terrain::Terrain()
{
	xPxl = 0;
	zPxl = 0;
	yPxl = 0;
	
	scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	heightMap = NULL;
	LPDIRECT3DDEVICE9 d3dDevice;
	for (int a = 0; a < MAX_TERRAIN_TEXTURE; ++a) texture[a] = NULL;
	vBuffer = NULL;
	iBuffer = NULL;
	triangles = 0;
	quadTree = NULL;
}

Terrain::~Terrain()
{

}

HRESULT Terrain::create(LPDIRECT3DDEVICE9 d3dDevice, Frustum* frustum, D3DXVECTOR3* scale, float ratio, LPSTR bmpName, LPSTR texName[MAX_TERRAIN_TEXTURE])
{
	this->d3dDevice = d3dDevice;
	this->scale = *scale;
	this->ratio = ratio;
	this->frustum = frustum;

	if (FAILED(buildHeightMap(bmpName)))
	{	
		MessageBox(NULL, L"Could not find map.bmp", L"Meshes.exe", MB_OK);
		destroy();
		return E_FAIL;
	}
	if (FAILED(loadTextures(texName)))
	{
		MessageBox(NULL, L"Could not find grass.jpg", L"Meshes.exe", MB_OK);
		destroy();
		return E_FAIL;
	}
	if (FAILED(createVIB()))
	{
		destroy();
		return E_FAIL;
	}

	quadTree = new QuadTree(xPxl, zPxl);

	if (FAILED(buildQuadTree()))
	{
		destroy();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Terrain::destroy()
{
	delete quadTree;
	delete[] heightMap;
	vBuffer->Release();
	iBuffer->Release();
	for (int a = 0; a < MAX_TERRAIN_TEXTURE; ++a) texture[a]->Release();
	return S_OK;
}

HRESULT Terrain::loadTextures(LPSTR texName[MAX_TERRAIN_TEXTURE])
{
	for (int a = 0; a < MAX_TERRAIN_TEXTURE; ++a)
	{
		D3DXCreateTextureFromFileA(d3dDevice, texName[a], &texture[a]);
	}

	return S_OK;
}

HRESULT	Terrain::buildHeightMap(LPSTR bmpname)
{
	int		n;
	LPBYTE	pDIB = DibLoadHandle(bmpname);
	if (!pDIB) return E_FAIL;

	xPxl = DIB_CX(pDIB)+1;
	zPxl = DIB_CY(pDIB)+1;

	// 새롭게 추가된 루틴
	// m_cxDIB나 m_czDIB가 (2^n+1)이 아닌경우 E_FAIL을 반환
	n = Log2(xPxl);
	if ((Pow2(n) + 1) != xPxl) return E_FAIL;
	n = Log2(zPxl);
	if ((Pow2(n) + 1) != zPxl) return E_FAIL;

	heightMap = new TERRAINVERTEX[xPxl * zPxl];

	TERRAINVERTEX v;
	for (int z = 0; z < zPxl; z++)
	{
		for (int x = 0; x < xPxl; x++)
		{
			v.p.x = (float)((x - xPxl / 2) * scale.x);
			v.p.z = -(float)((z - zPxl / 2) * scale.z);
			v.p.y = (float)(*(DIB_DATAXY_INV(pDIB, x, z))) * scale.y;
			D3DXVec3Normalize(&v.n, &v.p);
			v.t.x = (float)x / (float)(xPxl - 1);
			v.t.y = (float)z / (float)(zPxl - 1);
			heightMap[x + z * zPxl] = v;
		}
	}

	DibDeleteHandle(pDIB);
	return S_OK;
}

HRESULT	Terrain::buildQuadTree()
{
	quadTree->build(heightMap);
	return S_OK;
}

HRESULT	Terrain::createVIB()
{
	// VB생성
	if (FAILED(d3dDevice->CreateVertexBuffer(xPxl*zPxl * sizeof(TERRAINVERTEX),
		0, TERRAINVERTEX::FVF, D3DPOOL_DEFAULT, &vBuffer, NULL)))
	{
		destroy();
		return E_FAIL;
	}
	VOID* pVertices;
	if (FAILED(vBuffer->Lock(0, xPxl*zPxl * sizeof(TERRAINVERTEX), (void**)&pVertices, 0)))
	{
		destroy();
		return E_FAIL;
	}
	memcpy(pVertices, heightMap, xPxl*zPxl * sizeof(TERRAINVERTEX));
	vBuffer->Unlock();

	// IB생성
	if (FAILED(d3dDevice->CreateIndexBuffer((xPxl - 1)*(zPxl - 1) * 2 * sizeof(TRIINDEX), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &iBuffer, NULL)))
	{
		destroy();
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Terrain::render()
{
	LPDWORD		pI;

	if (FAILED(iBuffer->Lock(0, (xPxl - 1)*(zPxl - 1) * 2 * sizeof(TRIINDEX), (void**)&pI, 0)))
		return E_FAIL;
	triangles =quadTree->generateIndex(pI, heightMap, frustum, ratio);
	iBuffer->Unlock();

	d3dDevice->SetTexture(0, texture[0]);								// 0번 텍스쳐 스테이지에 텍스쳐 고정(색깔맵)
	d3dDevice->SetTexture(1, texture[1]);								// 1번 텍스쳐 스테이지에 텍스쳐 고정(음영맵)
	d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 0번 텍스처 스테이지의 확대 필터
	d3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 0번 텍스처 스테이지의 확대 필터
	d3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);		// 0번 텍스처 : 0번 텍스처 인덱스 사용
	d3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);		// 1번 텍스처 : 0번 텍스처 인덱스 사용

	d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// MODULATE로 섞는다.
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// 텍스처
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// 정점색
	d3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);	// MODULATE2로 섞는다.
	d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// 텍스처
	d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);		// 현재색
	d3dDevice->SetStreamSource(0, vBuffer, 0, sizeof(TERRAINVERTEX));
	d3dDevice->SetFVF(TERRAINVERTEX::FVF);
	d3dDevice->SetIndices(iBuffer);
	if(triangles>0) d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, xPxl * zPxl, 0, triangles);

	return S_OK;
}

