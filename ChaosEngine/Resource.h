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
	virtual bool LoadToMemory(int width, int height, ILuint imageId) { return false; }
	virtual bool LoadToMemory(Mesh* mesh) { return false; }
	virtual bool LoadToMemory() { return false; }
	virtual void UnloadFromMemory() {}
	//Models
	virtual std::vector<Vertex> GetVertex() { return std::vector<Vertex>(); }
	virtual std::vector<uint> GetIndices() { return std::vector<uint>(); }
	virtual std::vector<Textures> GetTextures() { return std::vector<Textures>(); }
	virtual std::vector<float> GetTexCoords() { return std::vector<float>(); }
	//Textures
	virtual inline uint GetWidth() { return 0; }
	virtual inline uint GetHeight() { return 0; }
	virtual inline uint GetBytes() { return 0; }
	virtual inline GLuint GetTextureId() { return 0; }
public:
	u32 UID = 0;
	std::string assetsFile;
	std::string libraryFile;
	ResourceType type = ResourceType::UNKNOWN;
	uint referenceCount = 0;
	std::vector<theBuffer*>* aux;
	bool firstTime = true;
};