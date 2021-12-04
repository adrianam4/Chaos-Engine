#include "Application.h"
#include "ResourceMaterial.h"
#include "Parson/parson.h"
#include "mmgr.h"

ResourceMatertial::ResourceMatertial(u32 _UID)
{
	type = ResourceType::TEXTURE;
	UID = _UID;
}

ResourceMatertial::~ResourceMatertial()
{
}

ResourceType ResourceMatertial::GetType()
{
	return type;
}

u32 ResourceMatertial::GetUID()
{
	return UID;
}

const char* ResourceMatertial::GetAssetFile()
{
	return assetsFile.c_str();
}

const char* ResourceMatertial::GetLibraryFile()
{
	return libraryFile.c_str();
}

uint ResourceMatertial::GetReferenceCount()
{
	return referenceCount;
}

bool ResourceMatertial::IsLoadedToMemory()
{
	return isLoaded;
}

bool ResourceMatertial::LoadToMemory(int _width, int _height, GLuint _imageId)
{
	bool ret = false;
	if (_width != 0)
	{
		width = _width;
		height = _height;
		textureId = _imageId;
		bytes = App->fileSystem->GetFileSize(GetLibraryFile());
		isLoaded = true;
		ret = true;
	}
	return ret;
}

bool ResourceMatertial::LoadToMemory()
{
	MaterialImporter importer;
	std::vector<int> data = importer.ImportMaterial(GetLibraryFile(), false, this);
	width = data[1];
	height = data[2];
	textureId = data[0];
	bytes = App->fileSystem->GetFileSize(GetLibraryFile());
	isLoaded = true;

	return true;
}

void ResourceMatertial::UnloadFromMemory()
{
	glDeleteTextures(1, &textureId);
	width = 0;
	height = 0;
	bytes = 0;
	textureId = 0;
	isLoaded = false;
}

void ResourceMatertial::GenerateMeta()
{
	JSON_Value* user_data = json_value_init_object();

	metaData.uid = UID;
	json_object_set_number(json_object(user_data), "UID", metaData.uid);
	metaData.texturePath = libraryFile;
	json_object_set_string(json_object(user_data), "TexturePath", metaData.texturePath.c_str());
	json_object_set_boolean(json_object(user_data), "Alienifying", metaData.alienifying);
	json_object_set_boolean(json_object(user_data), "Blurring", metaData.blurring);
	json_object_set_boolean(json_object(user_data), "Contrast", metaData.contrast);
	json_object_set_boolean(json_object(user_data), "Equalization", metaData.equalization);
	json_object_set_boolean(json_object(user_data), "GammaCorrection", metaData.gammaCorrection);
	json_object_set_boolean(json_object(user_data), "Negativity", metaData.negativity);
	json_object_set_boolean(json_object(user_data), "Noise", metaData.noise);
	json_object_set_boolean(json_object(user_data), "Pixelization", metaData.pixelization);
	json_object_set_boolean(json_object(user_data), "Sharpering", metaData.sharpering);

	std::string savingPath = libraryFile + ".meta";
	json_serialize_to_file_pretty(user_data, savingPath.c_str());

	App->editor->AddLog("Generated Texture Meta Data\n");
}

void ResourceMatertial::GenerateMeta(bool alienifying, bool blurring, bool contrast, bool equalization, bool gammaCorrection, bool negativity, bool noise, bool pixelization, bool sharpering)
{
	JSON_Value* user_data = json_value_init_object();

	metaData.uid = UID;
	json_object_set_number(json_object(user_data), "UID", metaData.uid);
	metaData.texturePath = libraryFile;
	json_object_set_string(json_object(user_data), "TexturePath", metaData.texturePath.c_str());
	json_object_set_boolean(json_object(user_data), "Alienifying", alienifying);
	json_object_set_boolean(json_object(user_data), "Blurring", blurring);
	json_object_set_boolean(json_object(user_data), "Contrast", contrast);
	json_object_set_boolean(json_object(user_data), "Equalization", equalization);
	json_object_set_boolean(json_object(user_data), "GammaCorrection", gammaCorrection);
	json_object_set_boolean(json_object(user_data), "Negativity", negativity);
	json_object_set_boolean(json_object(user_data), "Noise", noise);
	json_object_set_boolean(json_object(user_data), "Pixelization", pixelization);
	json_object_set_boolean(json_object(user_data), "Sharpering", sharpering);

	std::string savingPath = libraryFile + ".meta";
	json_serialize_to_file_pretty(user_data, savingPath.c_str());

	App->editor->AddLog("Generated Texture Meta Data\n");
}

void ResourceMatertial::LoadMeta()
{
	std::string loadingPath = libraryFile + ".meta";
	JSON_Value* userData = json_parse_file(loadingPath.c_str());

	if (userData != nullptr)
	{
		metaData.uid = json_object_get_number(json_object(userData), "UID");
		metaData.texturePath = json_object_get_string(json_object(userData), "TexturePath");
		metaData.alienifying = json_object_get_boolean(json_object(userData), "Alienifying");
		metaData.blurring = json_object_get_boolean(json_object(userData), "Blurring");
		metaData.contrast = json_object_get_boolean(json_object(userData), "Contrast");
		metaData.equalization = json_object_get_boolean(json_object(userData), "Equalization");
		metaData.gammaCorrection = json_object_get_boolean(json_object(userData), "GammaCorrection");
		metaData.negativity = json_object_get_boolean(json_object(userData), "Negativity");
		metaData.noise = json_object_get_boolean(json_object(userData), "Noise");
		metaData.pixelization = json_object_get_boolean(json_object(userData), "Pixelization");
		metaData.sharpering = json_object_get_boolean(json_object(userData), "Sharpering");
	}
}
