#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MeshComponent.h"

ComponentMesh::ComponentMesh(ComponentType mType, float3* pos, float3* measures)
{
	type = mType;

	switch (mType)
	{
	case ComponentType::MESH:
		App->renderer3D->InitMesh("Assets/BakerHouse.fbx", App->scene->gameObjects[App->scene->gameObjects.size() - 1]);
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
}