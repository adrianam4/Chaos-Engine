#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MeshComponent.h"

ComponentMesh::ComponentMesh(ComponentType mType, float3* pos, float3* measures)
{
	type = mType;

	switch (mType)
	{
	case ComponentType::MESH:
		App->renderer3D->InitMesh("Assets/Warrior.fbx", App->scene->gameObjects[App->scene->gameObjects.size() - 1]);
		App->renderer3D->models[App->renderer3D->models.size() - 1].id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::CUBE:
		App->editor->AddCube(*pos,*measures);
		App->editor->cubes[App->editor->cubes.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::PYRAMID:
		App->editor->AddPyramid(*pos, *measures);
		App->editor->pyramids[App->editor->pyramids.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::SPHERE:
		App->editor->AddSphere(*pos, *measures);
		App->editor->spheres[App->editor->spheres.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::PLANE:
		App->editor->AddPlane(float3(0, 0, 0), float3(1, 1, 1));
		App->editor->planes[App->editor->planes.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::CYLINDER:
		App->editor->AddCylinder(*pos);
		App->editor->cylinders[App->editor->cylinders.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	default:
		break;
	}

	name = "Mesh Component";
}

ComponentMesh::ComponentMesh(ComponentType mType, char* mName)
{
	type = mType;

	App->renderer3D->InitMesh(mName, App->scene->gameObjects[App->scene->gameObjects.size() - 1]);
	App->renderer3D->models[App->renderer3D->models.size() - 1].id = App->editor->lastId + 1;
	App->editor->lastId++;

	name = "Mesh Component";
}

ComponentMesh::~ComponentMesh()
{
}

void ComponentMesh::Enable()
{
}

void ComponentMesh::Update()
{
}

void ComponentMesh::Disable()
{
}

void ComponentMesh::OnEditor(int i)
{
	ImGui::Checkbox("Active", &App->editor->objectSelected->components[i]->active);

	if ((App->editor->objectSelected->components[i]->type == ComponentType::MESH))
	{
		int id = App->editor->objectSelected->id;
		int j;
		for (j = 0; j < App->renderer3D->models.size(); j++)
		{
			if (id == App->renderer3D->models[j].id)
			{
				break;
			}
		}
		ImGui::Text("Number of Meshes: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->renderer3D->models[j].meshes.size());
		ImGui::Text("Number of Faces: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->renderer3D->models[j].numFaces);
		ImGui::Text("Number of Vertices: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->renderer3D->models[j].meshes[i].vertices.size());
	}
	if ((App->editor->objectSelected->components[i]->type == ComponentType::CUBE)) 
	{
		int id = App->editor->objectSelected->id;
		int j;
		for (j = 0; j < App->editor->cubes.size(); j++)
		{
			if (id == App->editor->cubes[j]->id) 
			{
				break;
			}
		}
		ImGui::Text("Number of Meshes: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "1");
		ImGui::Text("Number of Faces: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->cubes[j]->indices.size() / 3);
		ImGui::Text("Number of Vertices: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->cubes[j]->vertices.size());
	}
	if ((App->editor->objectSelected->components[i]->type == ComponentType::PYRAMID))
	{
		int id = App->editor->objectSelected->id;
		int j;
		for (j = 0; j < App->editor->pyramids.size(); j++)
		{
			if (id == App->editor->pyramids[j]->id)
			{
				break;
			}
		}
		ImGui::Text("Number of Meshes: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "1");
		ImGui::Text("Number of Faces: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->pyramids[j]->indices.size() / 3);
		ImGui::Text("Number of Vertices: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->pyramids[j]->vertices.size());
	}
	if ((App->editor->objectSelected->components[i]->type == ComponentType::CYLINDER))
	{
		int id = App->editor->objectSelected->id;
		int j;
		for (j = 0; j < App->editor->cylinders.size(); j++)
		{
			if (id == App->editor->cylinders[j]->id)
			{
				break;
			}
		}
		ImGui::Text("Number of Meshes: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "1");
		ImGui::Text("Number of Faces: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->cylinders[j]->indices.size() / 3);
		ImGui::Text("Number of Vertices: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->cylinders[j]->vertices.size());
	}
	if ((App->editor->objectSelected->components[i]->type == ComponentType::SPHERE))
	{
		int id = App->editor->objectSelected->id;
		int j;
		for (j = 0; j < App->editor->spheres.size(); j++)
		{
			if (id == App->editor->spheres[j]->id)
			{
				break;
			}
		}
		ImGui::Text("Number of Meshes: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "1");
		ImGui::Text("Number of Faces: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->spheres[j]->indices.size() / 4);
		ImGui::Text("Number of Vertices: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->spheres[j]->vertices.size());
	}
	if ((App->editor->objectSelected->components[i]->type == ComponentType::PLANE))
	{
		int id = App->editor->objectSelected->id;
		int j;
		for ( j = 0; j < App->editor->planes.size(); j++)
		{
			if (id == App->editor->planes[j]->id)
			{
				break;
			}
		}
		ImGui::Text("Number of Meshes: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "1");
		ImGui::Text("Number of Faces: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->planes[j]->indices.size() / 3);
		ImGui::Text("Number of Vertices: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(255, 255, 0, 255), "%d", App->editor->planes[j]->vertices.size());
	}
}