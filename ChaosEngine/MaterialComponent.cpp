#include "Application.h"
#include "MaterialComponent.h"
#include "ResourceMaterial.h"

#include "GL/glew.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#include "Parson/parson.h"


ComponentMaterial::ComponentMaterial(ComponentType _type, const char* _path, bool _isDropped)
{
	type = _type;
	UID = GenerateUID();
	name = "Material Component";
	texturePath = _path;
	isDropped = _isDropped;

	std::vector<int> aux = importer.LoadMaterial(std::string(texturePath), isDropped);
	myImageId = aux[0];
	width = aux[1];
	height = aux[2];
	if (texturePath != importer.ddsPath.c_str())
		texturePath = importer.ddsPath.c_str();
	TexturePathConst = texturePath;
	showCheckerTexture = false;
}

ComponentMaterial::ComponentMaterial(const char* _path, GLuint _imageId, uint _width, uint _height, bool _isDropped)
{
	type = ComponentType::MATERIAL;
	UID = GenerateUID();
	name = "Material Component";
	texturePath = _path;
	isDropped = _isDropped;
	myImageId = _imageId;
	width = _width;
	height = _height;
	TexturePathConst = texturePath;
	showCheckerTexture = false;
}

ComponentMaterial::~ComponentMaterial()
{
	ilDeleteImage(myImageId);
}

void ComponentMaterial::Enable()
{
}

void ComponentMaterial::Update()
{
	if (showCheckerTexture)
	{
		std::string checkersPath = "Library/Textures/Checker.dds";
		std::vector<int> aux = importer.ImportMaterial(checkersPath, isDropped, nullptr);
		myImageId = aux[0];
		width = aux[1];
		height = aux[2];
	}
	else
	{
		std::vector<int> aux = importer.ImportMaterial(TexturePathConst, isDropped, nullptr);
		myImageId = aux[0];
		width = aux[1];
		height = aux[2];
	}
}

void ComponentMaterial::Disable()
{
}

void ComponentMaterial::OnEditor(int i)
{
	if (ImGui::Checkbox("Checkers Texture", &showCheckerTexture))
	{
		Update();
	}
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Path: "); ImGui::SameLine(); ImGui::Text(texturePath);
	ImGui::Image((void*)(intptr_t)myImageId, ImVec2(200, 200));
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Width: "); ImGui::SameLine(); ImGui::Text("%d", width);
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Height: "); ImGui::SameLine(); ImGui::Text("%d", height);
}

void ComponentMaterial::Load(const char* path)
{
	//Reading JSON File
	JSON_Value* userData = json_parse_file(path);

	UID = json_object_get_number(json_object(userData), "UID");
	showCheckerTexture = json_object_get_boolean(json_object(userData), "Chekers");

	App->editor->AddLog("Loaded Material Component Data\n");
}

void ComponentMaterial::Save(const char* path)
{
	//Creating Json file
	JSON_Value* user_data = json_value_init_object();

	json_object_set_number(json_object(user_data), "UID", UID);
	json_object_set_boolean(json_object(user_data), "Checkers", showCheckerTexture);

	json_serialize_to_file_pretty(user_data, path);

	App->editor->AddLog("Saved Material Component Data\n");
}

void ComponentMaterial::DeleteImage()
{
	ilDeleteImage(myImageId);
}
