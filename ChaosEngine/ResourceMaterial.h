#pragma once
#include "Resource.h"
#include "Globals.h"
#include "MaterialComponent.h"

class Meta
{
	u32 UID;
};

class ResourceMatertial : public Resource
{
public:
	ResourceMatertial(u32 UID);
	~ResourceMatertial();

	ResourceType GetType() override;
	u32 GetUID() override;
	const char* GetAssetFile() override;
	const char* GetLibraryFile() override;
	uint GetReferenceCount() override;
	bool IsLoadedToMemory() override;
	bool LoadToMemory(int width, int height, GLuint imageId) override;
	bool LoadToMemory() override;
	void UnloadFromMemory() override;
	inline uint GetWidth() override { return width; }
	inline uint GetHeight() override { return height; }
	inline uint GetBytes() override { return bytes; }
	inline GLuint GetTextureId() override { return textureId; }

public:
	uint width = 0;
	uint height = 0;
	uint bytes = 0;
	GLuint textureId = 0;
	bool isLoaded = false;
};