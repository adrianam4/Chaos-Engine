#include "Application.h" 
#include "MaterialImporter.h"
#include "ResourceMaterial.h"

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#include <string>

MaterialImporter::MaterialImporter()
{
}

MaterialImporter::~MaterialImporter()
{
}

void MaterialImporter::SaveMaterial(std::string sourcePath, std::string compression)
{
	ILuint size;
	ILubyte* data;

	ILint compressionMethod = IL_DXT5;
	if (compression == "IL_DXT_NO_COMP")
		compressionMethod = IL_DXT_NO_COMP;
	if (compression == "IL_DXT1")
		compressionMethod = IL_DXT1;
	if (compression == "IL_DXT2")
		compressionMethod = IL_DXT2;
	if (compression == "IL_DXT3")
		compressionMethod = IL_DXT3;
	if (compression == "IL_DXT4")
		compressionMethod = IL_DXT4;
	if (compression == "IL_DXT5")
		compressionMethod = IL_DXT5;

	ilSetInteger(IL_DXTC_FORMAT, compressionMethod);
	size = ilSaveL(IL_DDS, nullptr, 0);

	std::string auxPath = std::string(sourcePath);
	unsigned auxCreateLast = auxPath.find_last_of(".");
	std::string auxCreatePath = auxPath.substr(auxCreateLast, auxPath.length() - auxCreateLast);
	if (auxCreatePath != ".dds")
	{
		unsigned start = auxPath.find_last_of("\\");
		if (start > 10000)
			start = auxPath.find_last_of("/");
		auxPath = "Library/Textures/" + auxPath.substr(start + 1, auxPath.length() - start - 4) + "dds";

		if (size > 0)
		{
			data = new ILubyte[size];
			if (ilSaveL(IL_DDS, data, size) > 0)
				App->fileSystem->Save(auxPath.c_str(), data, size);

			delete[] data;
		}
	}
	ddsPath = auxPath;
}

std::vector<int> MaterialImporter::ImportMaterial(std::string sourcePath, bool isDropped, ResourceMatertial* resource)
{
	std::vector<int> toReturn;
	ILboolean success;

	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_2D, textId);
	toReturn.push_back(textId);

	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	success = ilLoadImage(sourcePath.c_str());

	if (resource != nullptr)
	{
		SaveMaterial(sourcePath.c_str(), resource->metaData.compression);

		if (resource->metaData.mipMap)
			iluBuildMipmaps();
		if (resource->metaData.alienifying)
			iluAlienify();
		if (resource->metaData.avgBlurring)
			iluBlurAvg(resource->metaData.amountAvBlur);
		if (resource->metaData.gausianBlurring)
			iluBlurGaussian(resource->metaData.amountGausianBlur);
		if (resource->metaData.contrast)
			iluContrast(resource->metaData.amountContrast);
		if (resource->metaData.equalization)
			iluEqualize();
		if (resource->metaData.gammaCorrection)
			iluGammaCorrect(resource->metaData.amountGammaCorrection);
		if (resource->metaData.negativity)
			iluNegative();
		if (resource->metaData.noise)
			iluNoisify(resource->metaData.amountNoise);
		if (resource->metaData.pixelization)
			iluPixelize(resource->metaData.amountPixelation);
		if (resource->metaData.sharpering)
			iluSharpen(resource->metaData.sharpenFactor, resource->metaData.sharpenIters);
	}
	else SaveMaterial(sourcePath.c_str(), "IL_DXT5");

	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	toReturn.push_back(w);
	toReturn.push_back(h);

	if (success)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
		{
			std::cout << "Could not convert image :: " << sourcePath << std::endl;
			ilDeleteImages(1, &imageId);
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = ilGetData();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned start = sourcePath.find_last_of(".");
	if (sourcePath.substr(start,sourcePath.length()-start) != ".dds")
	{
		glDeleteTextures(1, &textId);
	}
	ilDeleteImages(1, &imageId);

	int toFound;
	int a;

	if (!isDropped)
	{
		for (a = 0; a < App->renderer3D->models.size(); a++)
		{
			if (App->editor->objectSelected->id == App->renderer3D->models[a].id)
			{
				for (int b = 0; b < App->renderer3D->models[a].meshes.size(); b++)
				{
					glDeleteTextures(1, &App->renderer3D->models[a].meshes[b].textureId);
					ilDeleteImages(1, &imageId);
					App->renderer3D->models[a].meshes[b].textureId = textId;
				}
				break;
			}
		}
	}
	if (isDropped)
	{
		toFound = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::CUBE);

		if (toFound != -1)
		{
			for (a = 0; a < App->scene->gameObjects.size() - 1; a++)
			{
				if (App->editor->objectSelected->id == App->editor->cubes[a]->id)
				{
					break;
				}
			}
			glDeleteTextures(1, &App->editor->cubes[a]->aTextureId);
			ilDeleteImages(1, &App->editor->cubes[a]->imageID);
			App->editor->cubes[a]->aTextureId = textId;
		}
		else {
			toFound = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::CYLINDER);
			if (toFound != -1)
			{
				for (a = 0; a < App->scene->gameObjects.size() - 1; a++)
				{
					if (App->editor->objectSelected->id == App->editor->cylinders[a]->id)
					{
						break;
					}
				}
				glDeleteTextures(1, &App->editor->cylinders[a]->aTextureId);
				ilDeleteImages(1, &App->editor->cylinders[a]->imageID);
				App->editor->cylinders[a]->aTextureId = textId;
			}
			else {
				toFound = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::PYRAMID);
				if (toFound != -1)
				{
					for (a = 0; a < App->scene->gameObjects.size() - 1; a++)
					{
						if (App->editor->objectSelected->id == App->editor->pyramids[a]->id)
						{
							break;
						}
					}
					glDeleteTextures(1, &App->editor->pyramids[a]->aTextureId);
					ilDeleteImages(1, &App->editor->pyramids[a]->imageID);
					App->editor->pyramids[a]->aTextureId = textId;
				}
				else
				{
					toFound = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::SPHERE);
					if (toFound != -1)
					{
						for (a = 0; a < App->scene->gameObjects.size() - 1; a++)
						{
							if (App->editor->objectSelected->id == App->editor->spheres[a]->id)
							{
								break;
							}
						}
						glDeleteTextures(1, &App->editor->spheres[a]->aTextureId);
						ilDeleteImages(1, &App->editor->spheres[a]->imageID);
						App->editor->spheres[a]->aTextureId = textId;
					}
					else {
						toFound = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::MESH);
						if (toFound != -1)
						{
							for (a = 0; a < App->scene->gameObjects.size() - 1; a++)
							{
								if (App->editor->objectSelected->id == App->renderer3D->models[a].id)
								{
									break;
								}
							}

							for (int b = 0; b < App->renderer3D->models[a].meshes.size(); b++)
							{
								glDeleteTextures(1, &App->renderer3D->models[a].meshes[b].textureId);
								ilDeleteImages(1, &imageId);
								App->renderer3D->models[a].meshes[b].textureId = textId;
							}

						}
						else
						{
							toFound = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::PLANE);
							if (toFound != -1)
							{
								for (a = 0; a < App->scene->gameObjects.size() - 1; a++)
								{
									if (App->editor->objectSelected->id == App->editor->planes[a]->id)
									{

										break;
									}
								}
								glDeleteTextures(1, &App->editor->planes[a]->aTextureId);
								ilDeleteImages(1, &App->editor->planes[a]->imageID);
								App->editor->planes[a]->aTextureId = textId;
							}
						}
					}
				}
			}
		}
	}
	return toReturn;
}

std::vector<int> MaterialImporter::LoadMaterial(std::string sourcePath, bool isDropped)
{
	ImportMaterial(sourcePath, isDropped, nullptr);
	std::vector<int> aux = ImportMaterial(ddsPath, isDropped, nullptr);
	return aux;
}

std::vector<int> MaterialImporter::GetMaterialData()
{
	return std::vector<int>();
}

