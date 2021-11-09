#include "Application.h" 
#include "MaterialImporter.h"

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

void MaterialImporter::SaveMaterial(const char* sourcePath)
{
	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
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

void MaterialImporter::ImportMaterial(std::string sourcePath, int* width, int* height, bool isDropped, ILuint* myImageId)
{
	ILboolean success;
	ILuint imageID;
	GLuint textId;

	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_2D, textId);

	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	myImageId = &imageID;

	success = ilLoadImage(sourcePath.c_str());

	SaveMaterial(sourcePath.c_str());

	int w = ilGetInteger(IL_IMAGE_WIDTH);
	int h = ilGetInteger(IL_IMAGE_HEIGHT);
	width = &w;
	height = &h;

	if (success)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
		{
			std::cout << "Could not convert image :: " << sourcePath << std::endl;
			ilDeleteImages(1, &imageID);
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	glBindTexture(GL_TEXTURE_2D, 0);
	int toFound;
	int a;

	if (!isDropped)
	{
		for (int i = 0; i < App->renderer3D->models[App->renderer3D->models.size() - 1].meshes.size(); i++)
		{
			App->renderer3D->models[App->renderer3D->models.size() - 1].meshes[i].textureId = textId;
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
								ilDeleteImages(1, myImageId);
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
}

void MaterialImporter::LoadMaterial(std::string sourcePath, int* width, int* height, bool isDropped, ILuint* myImageId)
{
	ImportMaterial(sourcePath, width, height, isDropped, myImageId);
	ImportMaterial(ddsPath, width, height, isDropped, myImageId);
}

