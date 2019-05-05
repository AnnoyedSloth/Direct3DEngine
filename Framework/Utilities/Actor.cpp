#include "Actor.h"

Actor::Actor() : Object()
{
	mesh = NULL;
	materials = NULL;
	textures = NULL;
	numMaterials = 1;

	location = Vector3(0, 0, 0);
}

Actor::Actor(D3DXVECTOR3 &loc, D3DXVECTOR3 &rot, D3DXVECTOR3 &scale) : Object(loc, rot, scale)
{
	mesh = NULL;
	materials = NULL;
	textures = NULL;
	numMaterials = 1;

	location = loc;
	rotation = rot;
	this->scale = scale;
}

Actor::~Actor()
{
	if (materials) delete materials;
}

void Actor::initialize(LPDIRECT3DDEVICE9 d3dDevice)
{
	loadMesh(d3dDevice);
}

void Actor::beginPlay()
{
	isTickRunning = true;

}

void Actor::tick(float deltaTime)
{
	
}


HRESULT Actor::loadMesh(LPDIRECT3DDEVICE9 d3dDevice)
{
	this->d3dDevice = d3dDevice;
	LPD3DXBUFFER pD3DXMtrlBuffer;

	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX("Tiger.x", D3DXMESH_SYSTEMMEM,
		d3dDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &numMaterials,
		&mesh)))
	{
		// If model is not in current folder, try parent folder
		if (FAILED(D3DXLoadMeshFromX("..\\Tiger.x", D3DXMESH_SYSTEMMEM,
			d3dDevice, NULL,
			&pD3DXMtrlBuffer, NULL, &numMaterials,
			&mesh)))
		{
			MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
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
					MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
				}
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();

	return S_OK;
}


void Actor::render(float deltaTime)
{
	if(isTickRunning) tick(deltaTime);
	D3DXMATRIXA16 s;
	D3DXMatrixScaling(&s, scale.x, scale.y, scale.z);
	D3DXMATRIXA16 r;
	D3DXMatrixRotationYawPitchRoll(&r, D3DX_PI * rotation.x / 180.0f, D3DX_PI * rotation.y / 180.0f, D3DX_PI * rotation.z / 180.0f);
	D3DXMATRIXA16 l;
	D3DXMatrixTranslation(&l, location.x, location.y, location.z);

	worldMat = s * r * l;
	// Rendering of scene objects can happen here
	for (DWORD i = 0; i < numMaterials; i++)
	{
		// Set the material and texture for this subset
		d3dDevice->SetMaterial(&materials[i]);
		d3dDevice->SetTexture(0, textures[i]);

		// Draw the mesh subset
		mesh->DrawSubset(i);
	}

	// Set world transform based on object's world matrix
	d3dDevice->SetTransform(D3DTS_WORLD, &worldMat);
}

