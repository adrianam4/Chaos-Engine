#pragma once
#include "Resource.h"
#include "Globals.h"
#include "MaterialComponent.h"

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
	bool LoadToMemory() override;
	ComponentMaterial* GetMaterial();

private:
	ComponentMaterial* material;
};