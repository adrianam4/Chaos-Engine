#include "Application.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"

#include "MathGeoLib/src/MathGeoLib.h"

Component* GameObject::CreateComponent2(ComponentType type, float3 pos, double hFov, double nPlane, double fPlane)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::CAMERA:
		component = new ComponentCamera(pos, hFov, nPlane, fPlane);
		break;
	default:
		break;
	}

	return component;
}

Component* GameObject::CreateComponent(ComponentType type, const char* name, bool isDropped)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::MESH:
		component = new ComponentMesh(NULL, (char*)name);
		break;
	case ComponentType::MATERIAL:
		component = new ComponentMaterial(type, (char*)name, isDropped);
		break;
	default:
		break;
	}
	
		
	return component;
}
Component* GameObject::CreateMeshComponent(std::vector<theBuffer*>* theArray,const char* path) {
	Component* component = nullptr;
	component = new ComponentMesh(theArray, path);
	return component;
}
Component* GameObject::CreateComponent(ComponentType type,float3* pos, float3* measures, float3* rot)
{
	Component* component = nullptr;

	float3 position = *pos;
	float3 scale = *measures;
	float3 rotation = *rot;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		
		component = new ComponentTransform(position, scale, rotation);
		break;
	case ComponentType::MESH:
		component = new ComponentMesh(ComponentType::MESH);
		break;
	case ComponentType::CUBE:
		component = new ComponentMesh(ComponentType::CUBE, pos, measures);
		break;
	case ComponentType::CYLINDER:
		component = new ComponentMesh(ComponentType::CYLINDER, pos , measures);
		break;
	case ComponentType::PYRAMID:
		component = new ComponentMesh(ComponentType::PYRAMID, pos, measures);
		break;
	case ComponentType::SPHERE:
		component = new ComponentMesh(ComponentType::SPHERE, pos, measures);
		break;
	case ComponentType::PLANE:
		component = new ComponentMesh(ComponentType::PLANE, pos, measures);
		break;
	case ComponentType::EMPTY:
		component = new ComponentMesh(ComponentType::EMPTY, pos, &float3(1,1,1));
		break;
	default:
		break;
	}

	return component;
}

void GameObject::Update()
{

}

ComponentTransform* GameObject::getTransform() {
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == ComponentType::TRANSFORM) {
			return (ComponentTransform*)components[i];
		}

	}

}
int GameObject::SearchComponent(GameObject* gameObject, ComponentType type)
{
	for (int i = 0; i < gameObject->components.size(); i++)
	{
		if (gameObject->components[i]->type == type)
			return i;
	}
	return -1;
}

u32 GameObject::GenerateUID()
{
	LCG uidGenerator;
	return uidGenerator.IntFast();
}

std::vector<float3> GameObject::GetVertices(int _id)
{
	std::vector<float3> vertices;
	for (int i = 0; i < App->renderer3D->models.size(); i++)
	{
		if (App->renderer3D->models[i].id == _id)
		{
			for (int j = 0; j < App->renderer3D->models[i].meshes.size(); j++)
			{
				for (int k = 0; k < App->renderer3D->models[i].meshes[j].vertices.size(); k++)
				{
					vertices.push_back(App->renderer3D->models[i].meshes[j].vertices[k].position);
				}
			}
		}
	}
	return vertices;
}

std::vector<float3> GameObject::GetNormals(int _id)
{
	std::vector<float3> normals;
	for (int i = 0; i < App->renderer3D->models.size(); i++)
	{
		if (App->renderer3D->models[i].id == _id)
		{
			for (int j = 0; j < App->renderer3D->models[i].meshes.size(); j++)
			{
				for (int k = 0; k < App->renderer3D->models[i].meshes[j].vertices.size(); k++)
				{
					normals.push_back(App->renderer3D->models[i].meshes[j].vertices[k].normal);
				}
			}
		}
	}
	return normals;
}
