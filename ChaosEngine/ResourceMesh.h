#pragma once
#include "Resource.h"
#include "Globals.h"
#include "MeshComponent.h"

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
	bool LoadToMemory() override;

	std::vector<float3> GetVerticesVector() const;
	std::vector<uint> GetIndicesVector() const;
private:
	std::vector<float3> vertices;
	std::vector<uint> indices;
public:
	uint* index;
	float* position;
	float* normal;
	float* texture;
};
