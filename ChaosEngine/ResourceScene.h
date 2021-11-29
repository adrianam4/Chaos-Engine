#pragma once
#include "Resource.h"
#include "Globals.h"

class ResourceScene : public Resource
{
public:
	ResourceScene(u32 UID);
	~ResourceScene();

	ResourceType GetType() override;
	u32 GetUID() override;
	const char* GetAssetFile() override;
	const char* GetLibraryFile() override;
	uint GetReferenceCount() override;
	bool IsLoadedToMemory() override;
	bool LoadToMemory() override;
	void UnloadFromMemory() override;
	//ComponentMaterial* GetMaterial();

private:
	//ComponentMaterial* material;
};