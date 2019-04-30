#pragma once
#include "Core/Core.h"

struct VertexData
{
	enum _FVF { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) };
	Vector3 pos;
	Vector3 normal;
	Vector3 binormal;
	Vector3 tangent;
	Vector2 uv;
};

struct MyIndex
{
	DWORD _0, _1, _2;
};