#include "Application.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		break;
		component = new ComponentTransform();
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
		break;
		component = new ComponentMaterial();
	default:
		break;
	}
	
	return component;
}

void GameObject::Update()
{

}
