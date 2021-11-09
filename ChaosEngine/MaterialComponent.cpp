#include "Application.h"
#include "MaterialComponent.h"

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

	importer.LoadMaterial(std::string(texturePath), &width, &height, isDropped, &myImageId);
	if (texturePath != importer.ddsPath.c_str())
		texturePath = importer.ddsPath.c_str();

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
			importer.ImportMaterial(std::string("Library/Textures/Checker.dds"), &width, &height, isDropped, &myImageId);
		}

	}
	else
	{
		importer.ImportMaterial(std::string(texturePath), &width, &height, isDropped, &myImageId);
	}
}

void ComponentMaterial::Disable()
{
}

void ComponentMaterial::OnEditor(int i)
{
	if (ImGui::Button("Save"))
		Save("Settings/MaterialComponent.json");
	ImGui::SameLine();
	if (ImGui::Button("Load"))
		Load("Settings/MaterialComponent.json");

	if (ImGui::Checkbox("Checkers Texture", &showCheckerTexture))
	{
		Update();
	}
	ImGui::TextColored(ImVec4(255, 255, 0, 255), "Path: "); ImGui::SameLine(); ImGui::Text(texturePath);
	ImGui::Image((void*)(intptr_t)myImageId, ImVec2(width/2, height/2));
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