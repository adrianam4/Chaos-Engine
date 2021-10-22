#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MeshComponent.h"

ComponentMesh::ComponentMesh(ComponentType mType)
{
	type = mType;

	switch (mType)
	{
	case ComponentType::MESH:
		App->renderer3D->InitMesh("Assets/BakerHouse.fbx", App->scene->game_objects[App->scene->game_objects.size() - 1]);
		App->renderer3D->models[App->renderer3D->models.size() - 1].id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::CUBE:
		App->editor->AddCube(float3(0,0,0),float3(1,1,1));
		App->editor->cubes[App->editor->cubes.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::PYRAMID:
		App->editor->AddPyramid(float3(0, 0, 0), float3(1, 1, 1));
		App->editor->pyramids[App->editor->pyramids.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::SPHERE:
		App->editor->AddSphere(float3(5,5,5),float3(1,1,1),3,20,20);
		App->editor->spheres[App->editor->spheres.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::CYLINDER:
		App->editor->AddCylinder(float3(0,0,0),float3(1,1,1));
		App->editor->cylinders[App->editor->cylinders.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	default:
		break;
	}

	name = "Mesh Component";
}

ComponentMesh::ComponentMesh(ComponentType mType, char* _name)
{
	type = mType;

	App->renderer3D->InitMesh(_name, App->scene->game_objects[App->scene->game_objects.size() - 1]);
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
