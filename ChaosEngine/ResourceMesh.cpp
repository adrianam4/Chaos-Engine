#include "Application.h"
#include "ResourceMesh.h"
#include "Parson/parson.h"

ResourceMesh::ResourceMesh(u32 _UID)
{
	type = ResourceType::MESH;
	UID = _UID;
}

ResourceMesh::~ResourceMesh()
{
	vertices.clear();
	indices.clear();
}

ResourceType ResourceMesh::GetType()
{
	return type;
}

u32 ResourceMesh::GetUID()
{
	return UID;
}

const char* ResourceMesh::GetAssetFile()
{
	return assetsFile.c_str();
}

const char* ResourceMesh::GetLibraryFile()
{
	return libraryFile.c_str();
}

uint ResourceMesh::GetReferenceCount()
{
	return referenceCount;
}

bool ResourceMesh::IsLoadedToMemory()
{
	return isLoaded;
}

bool ResourceMesh::LoadToMemory(Mesh* mesh)
{
	bool ret = false;

	if (mesh != nullptr)
	{
		for (int i = 0; i < mesh->vertices.size(); ++i)
		{
			vertices.push_back(mesh->vertices[i]);
		}
		for (int i = 0; i < mesh->indices.size(); ++i)
		{
			indices.push_back(mesh->indices[i]);
		}
		for (int i = 0; i < mesh->textures.size(); ++i)
		{
			textures.push_back(mesh->textures[i]);
		}
		for (int i = 0; i < mesh->texCoords.size(); ++i)
		{
			texCoords.push_back(mesh->texCoords[i]);
		}

		ret = true;
		isLoaded = true;
	}
	return ret;
}

void ResourceMesh::UnloadFromMemory()
{
	vertices.clear();
	indices.clear();
	textures.clear();
	texCoords.clear();

	isLoaded = false;
}

std::vector<Vertex> ResourceMesh::GetVertex()
{
	return vertices;
}

std::vector<uint> ResourceMesh::GetIndices()
{
	return indices;
}

std::vector<Textures> ResourceMesh::GetTextures()
{
	return textures;
}

std::vector<float> ResourceMesh::GetTexCoords()
{
	return texCoords;
}

void ResourceMesh::GenerateMeta()
{
	JSON_Value* user_data = json_value_init_object();

	metaData.uid = UID;
	json_object_set_number(json_object(user_data), "UID", metaData.uid);
	metaData.modelPath = libraryFile;
	json_object_set_string(json_object(user_data), "ModelPath", metaData.modelPath.c_str());
	json_object_set_boolean(json_object(user_data), "JoinVertex", metaData.joinVertex);
	json_object_set_boolean(json_object(user_data), "Triangulate", metaData.triangulate);
	json_object_set_boolean(json_object(user_data), "GenerateNormals", metaData.generateNormals);
	json_object_set_boolean(json_object(user_data), "GenerateSmoothNormals", metaData.generateSmoothNormals);
	json_object_set_boolean(json_object(user_data), "RemoveMaterials", metaData.removeMaterials);
	json_object_set_boolean(json_object(user_data), "InfacingNormals", metaData.infacingNormals);
	json_object_set_boolean(json_object(user_data), "GenUvCoords", metaData.genUvCoords);
	json_object_set_boolean(json_object(user_data), "TransUvCoords", metaData.transUvCoords);
	json_object_set_boolean(json_object(user_data), "FindInstances", metaData.findInstances);
	json_object_set_boolean(json_object(user_data), "OptimizeMesh", metaData.optimizeMesh);
	json_object_set_boolean(json_object(user_data), "FlipUvs", metaData.flipUvs);

	std::string savingPath = libraryFile + ".meta";
	json_serialize_to_file_pretty(user_data, savingPath.c_str());

	App->editor->AddLog("Generated Mesh Meta Data\n");
}

void ResourceMesh::GenerateMeta(bool joinVertex, bool triangulate, bool generateNormals, bool generateSmoothNormals, bool removeMaterials, bool infacingNormals, bool genUvCoords, bool transUvCoords, bool findInstances, bool optimizeMesh, bool flipUvs)
{
	JSON_Value* user_data = json_value_init_object();

	metaData.uid = UID;
	json_object_set_number(json_object(user_data), "UID", metaData.uid);
	metaData.modelPath = libraryFile;
	json_object_set_string(json_object(user_data), "ModelPath", metaData.modelPath.c_str());
	json_object_set_boolean(json_object(user_data), "JoinVertex", joinVertex);
	json_object_set_boolean(json_object(user_data), "Triangulate", triangulate);
	json_object_set_boolean(json_object(user_data), "GenerateNormals", generateNormals);
	json_object_set_boolean(json_object(user_data), "GenerateSmoothNormals", generateSmoothNormals);
	json_object_set_boolean(json_object(user_data), "RemoveMaterials", removeMaterials);
	json_object_set_boolean(json_object(user_data), "InfacingNormals", infacingNormals);
	json_object_set_boolean(json_object(user_data), "GenUvCoords", genUvCoords);
	json_object_set_boolean(json_object(user_data), "TransUvCoords", transUvCoords);
	json_object_set_boolean(json_object(user_data), "FindInstances", findInstances);
	json_object_set_boolean(json_object(user_data), "OptimizeMesh", optimizeMesh);
	json_object_set_boolean(json_object(user_data), "FlipUvs", flipUvs);

	std::string savingPath = libraryFile + ".meta";
	json_serialize_to_file_pretty(user_data, savingPath.c_str());

	App->editor->AddLog("Generated Mesh Meta Data\n");
}

void ResourceMesh::LoadMeta()
{
	std::string loadingPath = libraryFile + ".meta";
	JSON_Value* userData = json_parse_file(loadingPath.c_str());

	if (userData != nullptr)
	{
		metaData.uid = json_object_get_number(json_object(userData), "UID");
		metaData.modelPath = json_object_get_string(json_object(userData), "TexturePath");
		metaData.joinVertex = json_object_get_boolean(json_object(userData), "MipMap");
		metaData.triangulate = json_object_get_boolean(json_object(userData), "Alienifying");
		metaData.generateNormals = json_object_get_boolean(json_object(userData), "AverageBlurring");
		metaData.generateSmoothNormals = json_object_get_boolean(json_object(userData), "avBlurAmount");
		metaData.removeMaterials = json_object_get_boolean(json_object(userData), "GaussianBlurring");
		metaData.infacingNormals = json_object_get_boolean(json_object(userData), "gaussianBlurAmount");
		metaData.genUvCoords = json_object_get_boolean(json_object(userData), "Contrast");
		metaData.transUvCoords = json_object_get_boolean(json_object(userData), "contrastAmount");
		metaData.findInstances = json_object_get_boolean(json_object(userData), "Equalization");
		metaData.optimizeMesh = json_object_get_boolean(json_object(userData), "GammaCorrection");
		metaData.flipUvs = json_object_get_boolean(json_object(userData), "gammaCorrectionAmount");

		App->editor->AddLog("Loaded Mesh Meta Data\n");
	}
}


