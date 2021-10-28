#include "Application.h"
#include "MaterialComponent.h"

#include "GL/glew.h"
#include "DevIL/include/IL/il.h"
#include "DevIL/include/IL/ilu.h"
#include "DevIL/include/IL/ilut.h"


ComponentMaterial::ComponentMaterial(ComponentType _type, const char* _path, bool _isDropped)
{
	type = _type;
	name = "Material Component";
	path = _path;
	isDropped = _isDropped;
	loadTexture(_path);

	showCheckerTexture = false;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Enable()
{
}

void ComponentMaterial::Update()
{
	if (showCheckerTexture)
	{
		for (int i = 0; i < App->renderer3D->models[0].meshes.size(); i++)
		{
			loadTexture("Assets/Textures/Checker.png");
		}
	}
	else
	{
		loadTexture(path);
	}
}

void ComponentMaterial::Disable()
{
}

void ComponentMaterial::OnEditor(int i)
{
	if (ImGui::Checkbox("Checkers Texture", &showCheckerTexture))
		Update();
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Path: "); ImGui::SameLine(); ImGui::Text(path);
	ImGui::Image((void*)(intptr_t)myImageId, ImVec2(width/2, height/2));
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Width: "); ImGui::SameLine(); ImGui::Text("%d", width);
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Height: "); ImGui::SameLine(); ImGui::Text("%d", height);
}

bool ComponentMaterial::loadTexture(const char* _path)
{
	ILboolean success;
	ILuint imageID;
	width;
	height;
	GLuint textId;
	
	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_2D, textId);

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	myImageId = imageID;

	success = ilLoadImage(_path);

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	if (success)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
		{
			std::cout << "Could not convert image :: " << _path << std::endl;
			ilDeleteImages(1, &imageID);
		}
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

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
								ilDeleteImages(1, &myImageId);
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
	
	return true;
}