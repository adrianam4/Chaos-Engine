#pragma once
#include "Resource.h"
#include "Globals.h"
#include "MeshComponent.h"

struct MeshMeta
{
	u32 uid = 0;
	std::string modelPath = " ";
	bool joinVertex = false;
	bool triangulate = true;
	bool generateNormals = true;
	bool generateSmoothNormals = false;
	bool removeMaterials = false;
	bool infacingNormals = false;
	bool genUvCoords = true;
	bool transUvCoords = false;
	bool findInstances = false;
	bool optimizeMesh = true;
	bool flipUvs = true;

	bool isLoaded = false;
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
	void GenerateMeta(bool joinVertex, bool triangulate, bool generateNormals, bool generateSmoothNormals, bool removeMaterials, bool infacingNormals, bool genUvCoords, bool transUvCoords, bool findInstances, bool optimizeMesh, bool flipUvs);
	void LoadMeta() override;
public:
	std::vector<uint> indices;
	std::vector<Vertex> vertices;
	std::vector<float> texCoords;
	std::vector<Textures> textures;
	MeshMeta metaData;
	bool isLoaded = false;
};
