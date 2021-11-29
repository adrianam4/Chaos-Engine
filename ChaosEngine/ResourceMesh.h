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
	bool LoadToMemory(Mesh* mesh) override;
	void UnloadFromMemory() override;
	std::vector<Vertex> GetVerticesVector() const;
	std::vector<uint> GetIndicesVector() const;
private:
	std::vector<uint> indices;
	std::vector<Vertex> vertices;
	bool isLoaded = false;
};
