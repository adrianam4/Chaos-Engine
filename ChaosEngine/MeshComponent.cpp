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
		App->editor->AddPlane(*pos, *measures);
		App->editor->planes[App->editor->planes.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::CYLINDER:
		App->editor->AddCylinder(*pos, *measures);
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
	//Draw Mesh
	ImGui::Checkbox("Active", &App->editor->objectSelected->components[i]->active);
	//Draw Normals
	if (App->editor->objectSelected->components[i]->type == ComponentType::MESH)
	{
		if (ImGui::Checkbox("Normals", &App->editor->objectSelected->components[i]->drawNormals))
		{
			for (int j = 0; j < App->renderer3D->models.size(); ++j)
			{
				if (App->renderer3D->models[j].id == App->editor->objectSelected->id)
				{
					for (int k = 0; k < App->renderer3D->models[j].meshes.size(); ++k)
					{
						App->renderer3D->models[j].meshes[k].drawNormals = !App->renderer3D->models[j].meshes[k].drawNormals;
					}
					break;
				}
			}
		}
	}
	//Draw Wireframe

	if (ImGui::Checkbox("Wireframe", &App->editor->objectSelected->components[i]->drawWireframe))
	{
		if (App->editor->objectSelected->components[i]->type == ComponentType::MESH)
		{
			for (int j = 0; j < App->renderer3D->models.size(); ++j)
			{
				if (App->renderer3D->models[j].id == App->editor->objectSelected->id)
				{
					for (int k = 0; k < App->renderer3D->models[j].meshes.size(); ++k)
					{
						App->renderer3D->models[j].meshes[k].drawWireframe = !App->renderer3D->models[j].meshes[k].drawWireframe;
					}
					break;
				}
			}
		}
		else if (App->editor->objectSelected->components[i]->type == ComponentType::CUBE)
		{
			for (int j = 0; j < App->editor->cubes.size(); ++j)
			{
				if (App->editor->cubes[j]->id == App->editor->objectSelected->id)
				{
					App->editor->cubes[j]->wireframe = !App->editor->cubes[j]->wireframe;
					break;
				}
			}
		}
		else if (App->editor->objectSelected->components[i]->type == ComponentType::CYLINDER)
		{
			for (int j = 0; j < App->editor->cylinders.size(); ++j)
			{
				if (App->editor->cylinders[j]->id == App->editor->objectSelected->id)
				{
					App->editor->cylinders[j]->wireframe = !App->editor->cylinders[j]->wireframe;
					break;
				}
			}
		}
		else if (App->editor->objectSelected->components[i]->type == ComponentType::PYRAMID)
		{
			for (int j = 0; j < App->editor->pyramids.size(); ++j)
			{
				if (App->editor->pyramids[j]->id == App->editor->objectSelected->id)
				{
					App->editor->pyramids[j]->wireframe = !App->editor->pyramids[j]->wireframe;
					break;
				}
			}
		}
		else if (App->editor->objectSelected->components[i]->type == ComponentType::SPHERE)
		{
			for (int j = 0; j < App->editor->spheres.size(); ++j)
			{
				if (App->editor->spheres[j]->id == App->editor->objectSelected->id)
				{
					App->editor->spheres[j]->wireframe = !App->editor->spheres[j]->wireframe;
					break;
				}
			}
		}
		else if (App->editor->objectSelected->components[i]->type == ComponentType::PLANE)
		{
			for (int j = 0; j < App->editor->planes.size(); ++j)
			{
				if (App->editor->planes[j]->id == App->editor->objectSelected->id)
				{
					App->editor->planes[j]->wireframe = !App->editor->planes[j]->wireframe;
					break;
				}
			}
		}
	}

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

void ComponentMesh::Load(const char* path)
{
}

void ComponentMesh::Save(const char* path)
{
}
