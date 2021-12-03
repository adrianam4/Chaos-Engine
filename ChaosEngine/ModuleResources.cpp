#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"

ModuleResources::ModuleResources(Application* app, bool startEnabled) : Module(app, startEnabled)
{
}

ModuleResources::~ModuleResources()
{
	CleanUp();
}

u32 ModuleResources::Find(const char* fileInAssets) // OK
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

u32 ModuleResources::ImportFile(const char* newFileInAssets) // OK
{
	ResourceType type = GetResourceType(newFileInAssets);
	Resource* resource = CreateNewResource(newFileInAssets, type);
	u32 ret = 0;
	if (resource != nullptr)
	{
		char* fileBuffer = nullptr;
		unsigned int buffer = App->fileSystem->Load(newFileInAssets, &fileBuffer);
		FBXimporter meshImporter;
		MaterialImporter texImporter;

		if (type == ResourceType::MESH)
			resource->aux = meshImporter.saveToOurFile(newFileInAssets, "Library/Models/");
		else if (type == ResourceType::TEXTURE)
			texImporter.ImportMaterial(newFileInAssets, false);

		SaveResource(resource, newFileInAssets);
		ret = resource->GetUID();
		RELEASE_ARRAY(fileBuffer);
	}
	return ret;
}

u32 ModuleResources::GenerateUID() // OK
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

void ModuleResources::ReleaseResource(u32 UID) // OK
{
	Resource* toDeleteResource = GetResource(UID);
	deleted.push_back(GetResource(UID));
	resources.erase(UID);
	bool hasRemovedFromAssets = App->fileSystem->Remove(toDeleteResource->GetAssetFile());
	bool hasRemovedFromLibrary = App->fileSystem->Remove(toDeleteResource->GetLibraryFile());
}

void ModuleResources::SaveUID()
{
}

void ModuleResources::LoadUID()
{
}

bool ModuleResources::Init() // OK
{
	App->editor->AddLog("Loading Resources...\n");
	bool ret = true;
	return ret;
}

bool ModuleResources::CleanUp() // OK
{
	App->editor->AddLog("Unloading Resources...\n");
	bool ret = true;
	resources.clear();
	deleted.clear();

	return ret;
}

Resource* ModuleResources::CreateNewResource(const char* assetsFile, ResourceType type) // OK
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

	return ret;
}

bool ModuleResources::LoadResource(u32 UID)
{
	bool ret = false;
	Resource* resourceToLoad = nullptr;
	resourceToLoad = GetResource(UID);
	if (resourceToLoad != nullptr)
	{
		GameObject* gObj = App->scene->gameObjects[App->scene->gameObjects.size() - 1];
		ResourceType type = resourceToLoad->GetType();
		if (type == ResourceType::MESH)
		{
			FBXimporter importer;
			std::vector<theBuffer*>* info;
			std::vector<Mesh*> meshInfo;

			std::string path = resourceToLoad->libraryFile;
			unsigned start = path.find_last_of("/") + 1;
			std::string pathUID = path.substr(start, 8);
			std::string pathMesh = path.substr(start + 8, 1);
			std::string pathNumMeshes = path.substr(start + 9, 1);

			info = importer.loadFromOurFile("Library/Models/", pathUID.c_str(), pathMesh.c_str(), pathNumMeshes.c_str(), ".msh"); // TODO
			for (int i = 0; i < info->size(); i++)
			{
				char* pointer = (*info)[i]->buffer;
				meshInfo.push_back(importer.getNewMeshFBX(pointer));
			}
			for (int i = 0; i < meshInfo.size(); i++)
			{
				resourceToLoad->LoadToMemory(meshInfo[i]);	
			}
			gObj->components.push_back(gObj->CreateComponentWithResource(resourceToLoad));
		}
		else if (type == ResourceType::TEXTURE)
		{
			resourceToLoad->LoadToMemory();
			gObj->components.push_back(gObj->CreateComponentWithResource(resourceToLoad));
		}

		ret = true;
	}

	return ret;
}

u32 ModuleResources::RecoveryFile(const char* file)
{
	ResourceType type = GetResourceType(file);
	Resource* resource = CreateNewResource(file, type);
	u32 ret = 0;
	char* fileBuffer = nullptr;
	unsigned int buffer = App->fileSystem->Load(file, &fileBuffer);

	resources[resource->GetUID()] = resource;
	resource->assetsFile = file;
	resource->libraryFile = file;

	ret = resource->GetUID();
	RELEASE_ARRAY(fileBuffer);

	return ret;
}

ImTextureID ModuleResources::LoadIcons(u32 UID)
{
	Resource* resourceToLoad = nullptr;
	resourceToLoad = GetResource(UID);
	resourceToLoad->LoadToMemory();
	return ImTextureID(resourceToLoad->GetTextureId());
}

Resource* ModuleResources::GetResource(u32 UID) // OK
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
	Resource* resourceToLoad = nullptr;
	//Find the library file
	return resourceToLoad;
}

std::string ModuleResources::GenerateLibraryFile(const char* assetsFile) // OK
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

	return fileName;
}

ResourceType ModuleResources::GetResourceType(const char* assetsFile) // OK
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

void ModuleResources::SaveResource(Resource* resource, std::string assetsFile) // OK
{
	if (resource != nullptr)
	{
		resources[resource->GetUID()] = resource;
		resource->assetsFile = assetsFile;

		if (resource->GetType() == ResourceType::MESH)
			resource->libraryFile = resource->aux[0][0]->libraryDir;
		else
			resource->libraryFile = GenerateLibraryFile(assetsFile.c_str());
	}
}
