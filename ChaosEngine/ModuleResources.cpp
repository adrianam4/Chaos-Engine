#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"

ModuleResources::ModuleResources(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

ModuleResources::~ModuleResources()
{
}

u32 ModuleResources::Find(const char* fileInAssets)
{
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		if (it->second->assetsFile == std::string(fileInAssets))
		{
			return it->first; //Return UID
		}
	}
	return GenerateUID();
}

u32 ModuleResources::ImportFile(const char* newFileInAssets)
{
	Resource* resource = CreateNewResource(newFileInAssets, ResourceType::UNKNOWN);
	u32 ret = 0;
	SDL_RWops* fileBuffer = App->fileSystem->Load(newFileInAssets);
	ResourceType type = resource->GetType();
	switch (type)
	{
	case ResourceType::MESH:
		break;
	case ResourceType::TEXTURE:
		break;
	default:
		break;
	}
	
	return ret;
}

u32 ModuleResources::GenerateUID()
{
	LCG uidGenerator;
	return uidGenerator.IntFast();
}

Resource* ModuleResources::RequestResource(u32 UID)
{
	std::map<u32, Resource*>::iterator it = resources.find(UID);
	if (it != resources.end())
	{
		it->second->referenceCount++;
		return it->second;
	}

	return TryToLoadResource();
}

void ModuleResources::ReleaseResource(u32 UID)
{
	resources.erase(UID);
}

bool ModuleResources::Init()
{
	return true;
}

update_status ModuleResources::PreUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleResources::Update(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

update_status ModuleResources::PostUpdate(float dt)
{
	return update_status::UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	return true;
}

Resource* ModuleResources::CreateNewResource(const char* assetsFile, ResourceType type)
{
	Resource* ret = nullptr;
	u32 uid = GenerateUID();
	switch (type)
	{
	case ResourceType::TEXTURE:
		ret = (Resource*) new ResourceMatertial(uid);
		break;
	case ResourceType::MESH:
		ret = (Resource*) new ResourceMesh(uid);
		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		resources[uid] = ret;
		ret->assetsFile = assetsFile;
		ret->libraryFile = GenerateLibraryFile(assetsFile);
	}

	return ret;
}

Resource* ModuleResources::TryToLoadResource()
{
	return nullptr;
}

std::string ModuleResources::GenerateLibraryFile(const char* assetsFile)
{
	std::string fileName = std::string(assetsFile);
	unsigned last = fileName.find_last_of(".");
	std::string createPath = fileName.substr(last, fileName.length() - last);

	if (createPath == ".png" || createPath == ".PNG")
	{
		unsigned start = fileName.find_last_of("\\");
		if (start > 10000)
			start = fileName.find_last_of("/");
		fileName = "Library/Textures/" + fileName.substr(start + 1, fileName.length() - start - 4) + "dds";
	}
	if (createPath == ".fbx" || createPath == ".FBX")
	{
		unsigned start = fileName.find_last_of("\\");
		if (start > 10000)
			start = fileName.find_last_of("/");
		fileName = "Library/Models/" + fileName.substr(start + 1, fileName.length() - start - 4) + "msh";
	}

	return fileName;
}
