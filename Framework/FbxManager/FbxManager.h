// To use FBX format import you must install Fbx SDK
// You can download Fbx SDK from
// https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2019-0

#include "fbxsdk.h"
#include "Core/Core.h"

class FbxLoadMgr
{
	FbxScene* scene;
public:
	BOOL importFbx(LPWSTR name);
	void loadNode(FbxNode* node);
};