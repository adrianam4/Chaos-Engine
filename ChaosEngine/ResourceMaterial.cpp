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
	return isLoaded;
}

bool ResourceMatertial::LoadToMemory(int _width, int _height, ILuint _imageId)
{
	bool ret = false;
	if (_width != 0)
	{
		width = _width;
		height = _height;
		gpuId = _imageId;
		bytes = App->fileSystem->GetFileSize(GetLibraryFile());
		isLoaded = true;
		ret = true;
	}
	return ret;
}

void ResourceMatertial::UnloadFromMemory()
{
	width = 0;
	height = 0;
	bytes = 0;
	gpuId = 0;
	isLoaded = false;
}

