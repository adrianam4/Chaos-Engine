#pragma once
#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>
#include "MathGeoLib/src/MathGeoLib.h"

enum class ResourceType
{
	TEXTURE = 1,
	MESH,
	SCENE,
	UNKNOWN,
	NONE,
};

class Resource
{
public:
	Resource();
	Resource(u32 _UID, ResourceType _type);
	~Resource();
	virtual ResourceType GetType() { return ResourceType::NONE; }
	virtual u32 GetUID() { return 0; }
	virtual const char* GetAssetFile() { return nullptr; }
	virtual const char* GetLibraryFile() { return nullptr; }
	virtual uint GetReferenceCount() { return 0; }
	virtual bool IsLoadedToMemory() { return false; }
	virtual bool LoadToMemory(int* width, int* height, ILuint* imageId) { return false; }
	virtual bool LoadToMemory(Mesh* mesh) { return false; }
	virtual bool LoadToMemory() { return false; }
	virtual void UnloadFromMemory() {}

public:
	u32 UID = 0;
	std::string assetsFile;
	std::string libraryFile;
	ResourceType type = ResourceType::UNKNOWN;
	uint referenceCount = 0;
};