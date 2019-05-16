#pragma once
#include "Core/Core.h"

struct BaseKey
{
	int frame;
};

struct KeyPos : public BaseKey
{
	Vector3 value;
};

struct KeyRot : public BaseKey
{
	Quaternion value;
};

struct KeyScl : public BaseKey
{
	Vector3 value;
};

struct Keys
{
	std::vector<KeyPos> pos;
	std::vector<KeyRot> rot;
	std::vector<KeyScl> scl;
};

struct RigidVertex
{
	enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	Vector3 p;
	Vector3 n;
	Vector3 t;
};

struct SkinnedVertex
{
	enum {FVF = (D3DFVF_XYZB4 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1)};
	Vector3 p;
	float b[3];
	DWORD i;
	Vector3 n;
	Vector3 t;

};

struct Track
{
	float start;
	float end;
	std::vector<KeyPos> pos;
	std::vector<KeyRot> rot;
	std::vector<KeyScl> scl;
	~Track(){}
};
