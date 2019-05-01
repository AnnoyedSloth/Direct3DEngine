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

// 2^n���� ���Ѵ�
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
	
	scale = D3DXVECTOR3(.3f, .01f, .3f);
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
	delete quadTree;
}

HRESULT Terrain::create(LPDIRECT3DDEVICE9 d3dDevice, Frustum* frustum, D3DXVECTOR3* scale, float ratioLOD, LPSTR bmpName, LPSTR texName[MAX_TERRAIN_TEXTURE])
{
	this->d3dDevice = d3dDevice;
	this->scale = *scale;
	this->ratioLOD = ratioLOD;
	this->frustum = frustum;

	if (FAILED(buildHeightMap(bmpName)))
	{	
		MessageBox(NULL, "Could not find map.bmp", "Meshes.exe", MB_OK);
		destroy();
		return E_FAIL;
	}
	if (FAILED(loadTextures(texName)))
	{
		MessageBox(NULL, "Could not find grass.png", "Meshes.exe", MB_OK);
		destroy();
		return E_FAIL;
	}
	if (FAILED(createVIB()))
	{
		MessageBox(NULL, "Could not create VIB", "Meshes.exe", MB_OK);
		destroy();
		return E_FAIL;
	}

	quadTree = new QuadTree(xPxl, zPxl);

	if (FAILED(buildQuadTree()))
	{
		MessageBox(NULL, "Could not build quadtree", "Meshes.exe", MB_OK);
		destroy();
		return E_FAIL;
	}

	materials = new D3DMATERIAL9();


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

	xPxl = DIB_CX(pDIB);
	zPxl = DIB_CY(pDIB);

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
	// VB����
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

#ifdef _USE_INDEX16
	// IB����
	if (FAILED(d3dDevice->CreateIndexBuffer((xPxl - 1)*(zPxl - 1) * 2 * sizeof(TRIINDEX), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &iBuffer, NULL)))
	{
		destroy();
		return E_FAIL;
	}
#else
	// IB����
	if (FAILED(d3dDevice->CreateIndexBuffer((xPxl - 1)*(zPxl - 1) * 2 * sizeof(TRIINDEX), 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &iBuffer, NULL)))
	{
		destroy();
		return E_FAIL;
	}
#endif
	return S_OK;
}

HRESULT Terrain::render()
{
	LPDWORD		pI;

	if (FAILED(iBuffer->Lock(0, (xPxl - 1)*(zPxl - 1) * 2 * sizeof(TRIINDEX), (void**)&pI, 0)))
		return E_FAIL;
	triangles =quadTree->generateIndex(pI, heightMap, frustum, ratioLOD);
	iBuffer->Unlock();

	D3DXMATRIXA16 pos;
	D3DXMatrixTranslation(&pos, 0, 0, 0);
	d3dDevice->SetTransform(D3DTS_WORLD, &pos);

	materials->Diffuse.r;//= materials->Ambient.r = 1.0f;
	materials->Diffuse.g;//= materials->Ambient.g = 1.0f;
	materials->Diffuse.b;//= materials->Ambient.b = 1.0f;
	materials->Diffuse.a;//= materials->Ambient.a = 1.0f;

	d3dDevice->SetMaterial(materials);
	d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	d3dDevice->SetTexture(0, texture[0]);								// 0�� �ؽ��� ���������� �ؽ��� ����(�����)
	d3dDevice->SetTexture(1, texture[1]);								// 1�� �ؽ��� ���������� �ؽ��� ����(������)
	d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 0�� �ؽ�ó ���������� Ȯ�� ����
	d3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 0�� �ؽ�ó ���������� Ȯ�� ����
	d3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);		// 0�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
	d3dDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);		// 1�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���

	d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// MODULATE�� ���´�.
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �ؽ�ó
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// ������
	d3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE2X);	// MODULATE2�� ���´�.
	d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �ؽ�ó
	d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);		// �����
	d3dDevice->SetStreamSource(0, vBuffer, 0, sizeof(TERRAINVERTEX));
	d3dDevice->SetFVF(TERRAINVERTEX::FVF);
	d3dDevice->SetIndices(iBuffer);
	d3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, xPxl * zPxl, 0, triangles);

	return S_OK;
}

