#include "Application.h"
#include "ResourceMesh.h"

ResourceMesh::ResourceMesh(u32 _UID)
{
	type = ResourceType::MESH;
	UID = _UID;
}

ResourceMesh::~ResourceMesh()
{
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
	return false;
}

bool ResourceMesh::LoadToMemory()
{
	return false;
}

std::vector<float3> ResourceMesh::GetVerticesVector() const
{
	return vertices;
}

std::vector<uint> ResourceMesh::GetIndicesVector() const
{
	return indices;
}
