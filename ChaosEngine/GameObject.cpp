#include "Application.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

Component* GameObject::CreateComponent(ComponentType type, const char* name)
{
	Component* component = nullptr;

	component = new ComponentMesh(ComponentType::MESH, (char*)name);
		
	return component;
}

Component* GameObject::CreateComponent(ComponentType type)
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
		component = new ComponentMesh(ComponentType::CUBE);
		break;
	case ComponentType::CYLINDER:
		component = new ComponentMesh(ComponentType::CYLINDER);
		break;
	case ComponentType::PYRAMID:
		component = new ComponentMesh(ComponentType::PYRAMID);
		break;
	case ComponentType::SPHERE:
		component = new ComponentMesh(ComponentType::SPHERE);
		break;
	case ComponentType::MATERIAL:
		component = new ComponentMaterial();
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
	return 0;
}