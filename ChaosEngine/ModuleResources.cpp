#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "ResourceScene.h"

ModuleResources::ModuleResources(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

ModuleResources::~ModuleResources()
{
	CleanUp();
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
	ResourceType type = GetResourceType(newFileInAssets);
	Resource* resource = CreateNewResource(newFileInAssets, type);
	u32 ret = 0;
	char** fileBuffer;
	unsigned int buffer = App->fileSystem->Load(newFileInAssets, fileBuffer);

	FBXimporter meshImporter;
	MaterialImporter texImporter;
	switch (type)
	{
	case ResourceType::MESH:
		meshImporter.saveToOurFile(newFileInAssets, GenerateLibraryFile(newFileInAssets).c_str()); // TODO
		break;
	case ResourceType::TEXTURE:
		texImporter.ImportMaterial(std::string(newFileInAssets), 0, 0, false, nullptr); // TODO
		break;
	case ResourceType::SCENE: 
		// TODO
		break;
	default:
		break;
	}
	SaveResource(resource, newFileInAssets);
	ret = resource->GetUID();
	delete[] fileBuffer;
	ReleaseResource(resource->GetUID());

	return ret;
}

u32 ModuleResources::GenerateUID()
{
	LCG uidGenerator;
	return uidGenerator.IntFast();
}

Resource* ModuleResources::RequestResource(u32 UID)
{
	Resource* resource = nullptr;
	resource = GetResource(UID);
	if (resource != nullptr)
	{
		return resource;
	}
	return TryToLoadResource(); // TODO
}

void ModuleResources::ReleaseResource(u32 UID)
{
	deleted.push_back(GetResource(UID));
	resources.erase(UID);
}

void ModuleResources::SaveUID()
{
}

void ModuleResources::LoadUID()
{
}

bool ModuleResources::Init()
{
	App->editor->AddLog("Loading Resources...\n");
	bool ret = true;
	return ret;
}

bool ModuleResources::CleanUp()
{
	App->editor->AddLog("Unloading Resources...\n");
	bool ret = true;

	for (auto it = resources.begin(); it != resources.end(); ++it)
	{
		delete[] it->second;
	}
	resources.clear();
	for (int i = 0; i < deleted.size(); ++i)
	{
		delete[] deleted[i];
	}
	deleted.clear();

	return ret;
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
	case ResourceType::SCENE:
		ret = (Resource*) new ResourceScene(uid);
		break;
	default:
		break;
	}

	if (ret != nullptr)
		SaveResource(ret, assetsFile);

	return ret;
}

Resource* ModuleResources::GetResource(u32 UID)
{
	std::map<u32, Resource*>::iterator it = resources.find(UID);
	if (it != resources.end())
	{
		it->second->referenceCount++;
		return it->second;
	}

	return nullptr;
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

ResourceType ModuleResources::GetResourceType(const char* assetsFile)
{
	std::string determinesType = std::string(assetsFile);
	unsigned start = determinesType.find_last_of(".");
	determinesType = determinesType.substr(start, determinesType.length() - start);
	if (determinesType == ".png" || determinesType == ".PNG" || determinesType == ".dds" || determinesType == ".DDS")
	{
		return ResourceType::TEXTURE;
	}
	if (determinesType == ".fbx" || determinesType == ".FBX" || determinesType == ".msh" || determinesType == ".MSH")
	{
		return ResourceType::MESH;
	}
	
	return ResourceType::NONE;
}

void ModuleResources::SaveResource(Resource* resource, std::string assetsFile)
{
	resources[resource->GetUID()] = resource;
	resource->assetsFile = assetsFile;
	resource->libraryFile = GenerateLibraryFile(assetsFile.c_str());
}
