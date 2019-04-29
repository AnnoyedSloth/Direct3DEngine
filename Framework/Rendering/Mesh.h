#pragma once
#include "fbxsdk.h"
#include "Core/Core.h"

class Mesh
{
public:
	Mesh();
	BOOL importFbx(LPWSTR name);
	void loadNode(FbxNode* node);
};