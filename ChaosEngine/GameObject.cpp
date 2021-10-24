#include "Application.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

Component* GameObject::CreateComponent(ComponentType type, const char* name)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::MESH:
		component = new ComponentMesh(type, (char*)name);
		break;
	case ComponentType::MATERIAL:
		component = new ComponentMaterial(type, (char*)name);
		break;
	default:
		break;
	}
	
		
	return component;
}

Component* GameObject::CreateComponent(ComponentType type,float3* pos, float3* measures)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		component = new ComponentTransform(App->scene->gameObjects[App->scene->gameObjects.size() - 1]->trans, App->scene->gameObjects[App->scene->gameObjects.size() - 1]->sca, App->scene->gameObjects[App->scene->gameObjects.size() - 1]->rot);
		break;
	case ComponentType::MESH:
		component = new ComponentMesh(ComponentType::MESH);
		break;
	case ComponentType::CUBE:
		component = new ComponentMesh(ComponentType::CUBE, pos, measures);
		break;
	case ComponentType::CYLINDER:
		component = new ComponentMesh(ComponentType::CYLINDER, pos);
		break;
	case ComponentType::PYRAMID:
		component = new ComponentMesh(ComponentType::PYRAMID, pos, measures);
		break;
	case ComponentType::SPHERE:
		component = new ComponentMesh(ComponentType::SPHERE, pos, measures);
		break;
	case ComponentType::PLANE:
		component = new ComponentMesh(ComponentType::PLANE, pos);
		break;
	case ComponentType::EMPTY:
		component = new ComponentMesh(ComponentType::EMPTY);
		break;
	default:
		break;
	}

	return component;
}

void GameObject::Update()
{

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