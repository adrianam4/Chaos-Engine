#pragma once
#include "Module.h"
#include "Globals.h"
#include "Resource.h"
#include <map>

class ModuleResources : public Module
{
public:
	ModuleResources(Application* app, bool startEnabled = true);
	~ModuleResources();

	u32 Find(const char* fileInAssets);
	u32 ImportFile(const char* newFileInAssets);
	u32 GenerateUID();

	Resource* RequestResource(u32 UID);
	void ReleaseResource(u32 UID);

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	Resource* CreateNewResource(const char* assetsFile, ResourceType type);
	Resource* TryToLoadResource();
	std::string GenerateLibraryFile(const char* assetsFile);
private:
	std::map<u32, Resource*> resources;
};