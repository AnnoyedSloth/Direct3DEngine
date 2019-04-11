#include "Object.h"



Object::Object(D3DXVECTOR3 &loc)
{
	mesh = NULL;
	materials = NULL;
	textures = NULL;
	numMaterials = 0L;

	location = loc;
}

Object::Object(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale)
{
	mesh = NULL;
	materials = NULL;
	textures = NULL;
	numMaterials = 0L;

	location = loc;
	rotation = rot;
	this->scale = scale;
}

Object::~Object()
{
	if (materials) delete materials;
}

HRESULT Object::LoadMesh(LPDIRECT3DDEVICE9 d3dDevice)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX(L"Tiger.x", D3DXMESH_SYSTEMMEM,
		d3dDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &numMaterials,
		&mesh)))
	{
		// If model is not in current folder, try parent folder
		if (FAILED(D3DXLoadMeshFromX(L"..\\Tiger.x", D3DXMESH_SYSTEMMEM,
			d3dDevice, NULL,
			&pD3DXMtrlBuffer, NULL, &numMaterials,
			&mesh)))
		{
			MessageBox(NULL, L"Could not find tiger.x", L"Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	materials = new D3DMATERIAL9[numMaterials];
	if (materials == NULL)
		return E_OUTOFMEMORY;
	textures = new LPDIRECT3DTEXTURE9[numMaterials];
	if (textures == NULL)
		return E_OUTOFMEMORY;

	for (DWORD i = 0; i < numMaterials; i++)
	{
		// Copy the material
		materials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		materials[i].Ambient = materials[i].Diffuse;

		textures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFileA(d3dDevice,
				d3dxMaterials[i].pTextureFilename,
				&textures[i])))
			{
				// If texture is not in current folder, try parent folder
				const CHAR* strPrefix = "..\\";
				CHAR strTexture[MAX_PATH];
				strcpy_s(strTexture, MAX_PATH, strPrefix);
				strcat_s(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFileA(d3dDevice,
					strTexture,
					&textures[i])))
				{
					MessageBox(NULL, L"Could not find texture map", L"Meshes.exe", MB_OK);
				}
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();

	return S_OK;
}

VOID Object::Render(LPDIRECT3DDEVICE9 d3dDevice)
{


	D3DXMATRIXA16 s;
	D3DXMatrixScaling(&s, scale.x, scale.y, scale.z);
	D3DXMATRIXA16 r;
	D3DXMatrixRotationYawPitchRoll(&r, rotation.x, rotation.y, rotation.z);
	D3DXMATRIXA16 l;
	D3DXMatrixTranslation(&l, location.x, location.y, location.z);


	D3DXMatrixIdentity(&worldMat);
	//D3DXMatrixMultiply(&worldMat, &worldMat, &s);
	//D3DXMatrixMultiply(&worldMat, &worldMat, &r);
	//D3DXMatrixMultiply(&worldMat, &worldMat, &l);

	worldMat = s * r * l;
	// Rendering of scene objects can happen here
	for (DWORD i = 0; i < numMaterials; i++)
	{
		// Set the material and texture for this subset
		//d3dDevice->SetTransform
		d3dDevice->SetMaterial(&materials[i]);
		d3dDevice->SetTexture(0, textures[i]);
		d3dDevice->SetTransform(D3DTS_WORLD, &worldMat);

		// Draw the mesh subset
		mesh->DrawSubset(i);
	}
}