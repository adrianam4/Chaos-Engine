#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MeshComponent.h"

ComponentMesh::ComponentMesh(ComponentType mType)
{
	type = mType;

	switch (mType)
	{
	case ComponentType::MESH:
		App->renderer3D->InitMesh("Assets/BakerHouse.fbx");
		App->renderer3D->models[App->renderer3D->models.size() - 1].id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::CUBE:
		App->editor->AddCube(-5,0,0,1,1,1);
		App->editor->cubes[App->editor->cubes.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::PYRAMID:
		App->editor->AddPyramid(5,0,0,1,1,1);
		App->editor->pyramids[App->editor->pyramids.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::SPHERE:
		App->editor->AddSphere(3,20,20);
		App->editor->spheres[App->editor->spheres.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	case ComponentType::CYLINDER:
		App->editor->AddCylinder();
		App->editor->cylinders[App->editor->cylinders.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;
		break;
	default:
		break;
	}

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
