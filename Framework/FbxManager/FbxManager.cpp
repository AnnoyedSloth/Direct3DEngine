#include "FbxManager.h"

BOOL FbxLoadMgr::importFbx(LPWSTR name)
{

	char* fileName = (char*)name;

	// Instantiate FBX Manager to import
	FbxManager* manager = FbxManager::Create();

	// Which can setup Fbx's Input/Output options
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	manager->GetIOSettings()->SetBoolProp(IMP_FBX_MODEL, true);
	manager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
	manager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
	manager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);


	FbxImporter* importer = FbxImporter::Create(manager, "");

	const bool status = importer->Initialize((char*)(fileName), -1, manager->GetIOSettings());

	if (!status)
	{
		MessageBox(NULL, "Could not find FBX File", "Meshes.exe", MB_OK);
		return false;
	}

	scene = FbxScene::Create(manager, fileName);
	importer->Import(scene);

	importer->Destroy();

	FbxNode* rootNode = scene->GetRootNode();

	loadNode(rootNode);
}

void FbxLoadMgr::loadNode(FbxNode* node)
{
	FbxNodeAttribute* nodeAttr = node->GetNodeAttribute();

	if (!nodeAttr) return;

	if (nodeAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* mesh = node->GetMesh();
	}

	const int childCount = node->GetChildCount();
	for (auto i = 0; i < childCount; ++i)
	{
		loadNode(node->GetChild(i));
	}

}