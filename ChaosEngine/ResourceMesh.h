#pragma once
#include "Resource.h"
#include "Globals.h"
#include "MeshComponent.h"

struct MeshMeta
{
	u32 uid = 0;
	std::string modelPath = " ";
	std::vector<Mesh> meshes;
	bool joinVertex = false;
	bool triangulate = true;
	bool generateNormals = false;
	bool generateSmoothNormals = false;
	bool removeMaterials = false;
	bool infacingNormals = false;
	bool genUvCoords = true;
	bool transUvCoords = false;
	bool findInstances = true;
	bool optimizeMesh = true;
	bool flipUvs = true;
};

class ResourceMesh : public Resource
{
public:
	ResourceMesh(u32 UID);
	~ResourceMesh();
	ResourceType GetType() override;
	u32 GetUID() override;
	const char* GetAssetFile() override;
	const char* GetLibraryFile() override;
	uint GetReferenceCount() override;
	bool IsLoadedToMemory() override;
	bool LoadToMemory(Mesh* mesh) override;
	void UnloadFromMemory() override;
	std::vector<Vertex> GetVertex() override;
	std::vector<uint> GetIndices() override;
	std::vector<Textures> GetTextures() override;
	std::vector<float> GetTexCoords() override;
	void GenerateMeta() override;
public:
	std::vector<uint> indices;
	std::vector<Vertex> vertices;
	std::vector<float> texCoords;
	std::vector<Textures> textures;
	MeshMeta metaData;
	bool isLoaded = false;
};
