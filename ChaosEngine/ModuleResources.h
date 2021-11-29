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

	bool Init();
	bool CleanUp();

	u32 Find(const char* fileInAssets);
	u32 ImportFile(const char* newFileInAssets);
	u32 GenerateUID();

	Resource* RequestResource(u32 UID);
	void ReleaseResource(u32 UID);
	void SaveUID();
	void LoadUID();
	Resource* CreateNewResource(const char* assetsFile, ResourceType type);
	bool LoadResource(u32 UID);
	Resource* GetResource(u32 UID);
	Resource* TryToLoadResource();
	std::string GenerateLibraryFile(const char* assetsFile);
	ResourceType GetResourceType(const char* assetsFile);
	void SaveResource(Resource* resource, std::string assetsFile);
private:
	std::map<u32, Resource*> resources;
	std::vector<Resource*> deleted;
	Component* componentToLoad;
};