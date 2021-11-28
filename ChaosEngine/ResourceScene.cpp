#include "Application.h"
#include "ResourceScene.h"

ResourceScene::ResourceScene(u32 _UID)
{
	type = ResourceType::SCENE;
	UID = _UID;
}

ResourceScene::~ResourceScene()
{
}

ResourceType ResourceScene::GetType()
{
	return type;
}

u32 ResourceScene::GetUID()
{
	return UID;
}

const char* ResourceScene::GetAssetFile()
{
	return assetsFile.c_str();
}

const char* ResourceScene::GetLibraryFile()
{
	return libraryFile.c_str();
}

uint ResourceScene::GetReferenceCount()
{
	return referenceCount;
}

bool ResourceScene::IsLoadedToMemory()
{
	return false;
}

bool ResourceScene::LoadToMemory()
{
	return false;
}
