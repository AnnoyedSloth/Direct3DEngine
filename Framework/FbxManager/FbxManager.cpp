#include "FbxManager.h"

FbxLoadMgr::FbxLoadMgr()
{
	vertices = 0;
}

FbxLoadMgr::~FbxLoadMgr()
{
	if(manager) manager->Destroy();
	//if(importer) importer->Destroy();
	delete[] vertexInfo;
	delete[] pos;
}

BOOL FbxLoadMgr::importFbx(const char* name)
{

	// Instantiate FBX Manager to import
	manager = FbxManager::Create();

	// Which can setup Fbx's Input/Output options

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	manager->GetIOSettings()->SetBoolProp(IMP_FBX_MODEL, true);
	manager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
	manager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
	manager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
	manager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);

	FbxImporter* importer = FbxImporter::Create(manager, "");

	const bool status = importer->Initialize(name, -1, manager->GetIOSettings());

	if (!status)
	{
		MessageBox(NULL, "Could not find FBX File", "Meshes.exe", MB_OK);
		return false;
	}

	scene = FbxScene::Create(manager, name);

	//scene->CreateAnimStack();
	//scene
	FbxStatus status;

	importer->Import(scene);

	importer->Destroy();

	FbxNode* rootNode = scene->GetRootNode();
	//FbxAnimStack myStack;

	loadNode(rootNode);
}

void FbxLoadMgr::loadNode(FbxNode* node)
{
	FbxNodeAttribute* nodeAttr = node->GetNodeAttribute();
	const char* name = node->GetName();

	// Root node does not have NodeAttribute. therefore ignore it in case of "RootNode"
	if (strcmp(name, "RootNode") != 0)
	{
		if (!nodeAttr) return;

		// In case of Node type is Mesh, move data into Vertex & Index buffer
		if (nodeAttr->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* mesh = node->GetMesh();
			processControlPoints(mesh);
		}
		if (nodeAttr->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			FbxSkeleton* skeleton = node->GetSkeleton();
		}
	}

	//FbxAnimCurveNode node;

	const int childCount = node->GetChildCount();
	for (unsigned int i = 0; i < childCount; ++i)
	{
		loadNode(node->GetChild(i));
	}

}

// Control point is identical to Indexed point
void FbxLoadMgr::processControlPoints(FbxMesh* mesh)
{
	// Count the number of Index points
	unsigned int indexedCount = indexes = mesh->GetControlPointsCount();

	// Dynamically allocate Vertex data as much as index points get
	pos = new Vector3[indexedCount];

	for (int i = 0; i < indexedCount; ++i)
	{
		pos[i].x = static_cast<float>
			(mesh->GetControlPointAt(i).mData[0]);
		pos[i].y = static_cast<float>
			(mesh->GetControlPointAt(i).mData[1]);
		pos[i].z = static_cast<float>
			(mesh->GetControlPointAt(i).mData[2]);
	}
	
	unsigned int polygonCount = mesh->GetPolygonCount();
	vertices = polygonCount * 3;

	// Dynamically allocate as much as amount of vertex demand
	vertexInfo = new VertexData[polygonCount*3];

	int vertexCounter = 0;

	for (unsigned int i = 0; i < polygonCount; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			int controlPointIdx = mesh->GetPolygonVertex(i, j);

			Vector3& position = pos[controlPointIdx];
			
			Vector3 normal = ReadNormal(mesh, controlPointIdx, vertexCounter);
			Vector3 binormal = ReadBinormal(mesh, controlPointIdx, vertexCounter);
			Vector3 tangent = ReadTangent(mesh, controlPointIdx, vertexCounter);
			Vector2 uv = ReadUV(mesh, controlPointIdx, vertexCounter, vertexCounter);

			vertexInfo[i * 3 + j] = { position, normal, binormal, tangent, uv };

			++vertexCounter;
		}
	}
}

// Referenced function by https://github.com/TheCherno/Sparky
Vector3 FbxLoadMgr::ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementNormalCount() < 1) return Vector3(0, 0, 0);

	Vector3 result;
	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	return result;
}

// Referenced function by https://github.com/TheCherno/Sparky
Vector2 FbxLoadMgr::ReadUV(const FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer)
{
	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer) return Vector2(0, 0);

	const FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);

	Vector2 result;
	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
		}
		break;
		}

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			result.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			result.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
		break;
		}
	}
	return result;
}

Vector3 FbxLoadMgr::ReadBinormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementBinormalCount() < 1) return Vector3(0, 0, 0);

	FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
	Vector3 result;
	switch (vertexBinormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	return result;
}

// Referenced function by https://github.com/TheCherno/Sparky
Vector3 FbxLoadMgr::ReadTangent(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	if (mesh->GetElementTangentCount() < 1) return Vector3(0, 0, 0);

	FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);
	Vector3 result;
	switch (vertexTangent->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(controlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(controlPointIndex);
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexTangent->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
		}
	}
	return result;
}