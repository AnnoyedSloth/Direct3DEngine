// To use FBX format import you must install Fbx SDK
// You can download Fbx SDK from
// https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2019-0

#include "fbxsdk.h"
#include "fbxsdk/scene/geometry/fbxnode.h"
#include "Rendering/VertexData.h"
#include "Core/Core.h"

class FbxLoadMgr
{
	FbxManager* manager;
	FbxIOSettings* ios;
	FbxImporter* importer;

	FbxScene* scene;
	Vector3* pos;
	VertexData* vertexInfo;
	unsigned int vertices;

public:
	FbxLoadMgr();
	~FbxLoadMgr();
	BOOL importFbx(const char* name);
	void loadNode(FbxNode* node);
	void processControlPoints(FbxMesh* mesh);

	Vector3 ReadNormal(const FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	Vector2 ReadUV(const FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer);
	Vector3 ReadBinormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	Vector3 ReadTangent(FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	VertexData* GetData() { return vertexInfo; }
};