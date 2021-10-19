#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MeshComponent.h"

ComponentMesh::ComponentMesh(ComponentType mType)
{
	type = mType;

	switch (mType)
	{
	case ComponentType::MESH:
		App->renderer3D->InitMesh("Assets/lowpolytree.fbx");
		break;
	case ComponentType::CUBE:
		App->editor->AddCube(-5,0,0,1,1,1);
		break;
	case ComponentType::PYRAMID:
		App->editor->AddPyramid(5,0,0,1,1,1);
		break;
	case ComponentType::SPHERE:
		App->editor->AddSphere(3,20,20);
		break;
	case ComponentType::CYLINDER:
		App->editor->AddCylinder();
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
