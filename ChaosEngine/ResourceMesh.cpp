#include "Application.h"
#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(u32 _UID)
{
	type = ResourceType::MESH;
	UID = _UID;
}

ResourceMesh::~ResourceMesh()
{
	vertices.clear();
	indices.clear();
}

ResourceType ResourceMesh::GetType()
{
	return type;
}

u32 ResourceMesh::GetUID()
{
	return UID;
}

const char* ResourceMesh::GetAssetFile()
{
	return assetsFile.c_str();
}

const char* ResourceMesh::GetLibraryFile()
{
	return libraryFile.c_str();
}

uint ResourceMesh::GetReferenceCount()
{
	return referenceCount;
}

bool ResourceMesh::IsLoadedToMemory()
{
	return isLoaded;
}

bool ResourceMesh::LoadToMemory(Mesh* mesh)
{
	bool ret = false;

	if (mesh != nullptr)
	{
		for (int i = 0; i < mesh->vertices.size(); ++i)
		{
			vertices.push_back(mesh->vertices[i]);
		}
		for (int i = 0; i < mesh->indices.size(); ++i)
		{
			indices.push_back(mesh->indices[i]);
		}
		ret = true;
		isLoaded = true;
	}
	return ret;
}

void ResourceMesh::UnloadFromMemory()
{
	vertices.clear();
	indices.clear();
	isLoaded = false;
}

std::vector<Vertex> ResourceMesh::GetVerticesVector() const
{
	return vertices;
}

std::vector<uint> ResourceMesh::GetIndicesVector() const
{
	return indices;
}
