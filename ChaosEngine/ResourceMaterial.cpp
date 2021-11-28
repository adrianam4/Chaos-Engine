#include "Application.h"
#include "ResourceMaterial.h"

ResourceMatertial::ResourceMatertial(u32 _UID)
{
	type = ResourceType::TEXTURE;
	UID = _UID;
}

ResourceMatertial::~ResourceMatertial()
{
}

ResourceType ResourceMatertial::GetType()
{
	return type;
}

u32 ResourceMatertial::GetUID()
{
	return UID;
}

const char* ResourceMatertial::GetAssetFile()
{
	return assetsFile.c_str();
}

const char* ResourceMatertial::GetLibraryFile()
{
	return libraryFile.c_str();
}

uint ResourceMatertial::GetReferenceCount()
{
	return referenceCount;
}

bool ResourceMatertial::IsLoadedToMemory()
{
	return false;
}

bool ResourceMatertial::LoadToMemory()
{
	return false;
}

ComponentMaterial* ResourceMatertial::GetMaterial()
{
	return material;
}
