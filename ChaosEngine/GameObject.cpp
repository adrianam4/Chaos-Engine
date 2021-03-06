#include "Application.h"
#include "SDL.h"
#include "GameObject.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "MaterialComponent.h"
#include "CameraComponent.h"
#include "ButtonComponent.h"
#include "CanvasComponent.h"
#include "ImageComponent.h"
#include "CheckBoxComponent.h"
#include "SliderComponent.h"
#include "InputBoxComponent.h"
#include "Resource.h"
#include "ResourceMaterial.h"
#include"Transform2DComponent.h"
#include "MathGeoLib.h"

Component* GameObject::CreateComponent2(ComponentType type, float3 pos, double hFov, double nPlane, double fPlane, bool isObj)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::CAMERA:
		component = new ComponentCamera(pos, hFov, nPlane, fPlane, isObj);
		break;
	default:
		break;
	}

	return component;
}

Component* GameObject::CreateUIComponent(ComponentType type, std::string text)
{
	Component* component = nullptr;

	switch (type)
	{
	case ComponentType::UI_BUTTON:
		component = new ButtonComponent(1, text);
		break;
	case ComponentType::UI_IMAGE:
		if (text == "Crosshair") component = new ImageComponent(10, text);
		else component = new ImageComponent(2, text);
		break;
	case ComponentType::UI_CHECKBOX:
		if (text == "V-Sync") 
			component = new CheckboxComponent(3, text);
		else 
			component = new CheckboxComponent(11, text);
		break;
	case ComponentType::UI_SLIDER:
		component = new SliderComponent(4, text);
		break;
	case ComponentType::UI_INPUTBOX:
		component = new InputBoxComponent(5, text);
		break;
	case ComponentType::UI_CANVAS:
		component = new CanvasComponent(6, text);
		break;
	default:
		break;
	}
	return component;
}

GameObject::~GameObject() 
{
	for (int a = 0; a < components.size(); a++) 
	{
		delete components[a];
	}
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

Component* GameObject::CreateMeshComponent(std::vector<theBuffer*>* theArray,const char* path) 
{
	Component* component = nullptr;
	component = new ComponentMesh(theArray, path);
	return component;
}

Component* GameObject::CreateComponentWithResource(Resource* resource)
{
	Component* component = nullptr;
	ResourceType type = resource->GetType();

	if (type == ResourceType::TEXTURE)
		component = new ComponentMaterial(resource->GetLibraryFile(), resource->GetTextureId(), resource->GetWidth(), resource->GetHeight(), false);
	else if (type == ResourceType::MESH)
		component = new ComponentMesh(resource->GetIndices(), resource->GetVertex(), resource->GetTextures(), resource->GetTexCoords(), resource->GetLibraryFile());
	
	return component;
}
Component* GameObject::CreateOneMeshComponent(theBuffer* temporal, const char* name) {
	Component* component = nullptr;
	component = new ComponentMesh(temporal);
	
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
	case ComponentType::TRANSFORM2D:

		component = new ComponentTransform2D(position, scale, rotation);
		break;
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

ComponentTransform* GameObject::getTransform() 
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == ComponentType::TRANSFORM) 
		{
			return (ComponentTransform*)components[i];
		}

	}

}

ComponentTransform2D* GameObject::getTransform2D()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == ComponentType::TRANSFORM2D)
		{
			return (ComponentTransform2D*)components[i];
		}

	}
}

ButtonComponent* GameObject::GetButtonComponent(GameObject* go)
{
	int comp = go->SearchComponent(go, ComponentType::UI_BUTTON);

	if (comp != -1)
		return (ButtonComponent*)go->components[comp];
	else
		return nullptr;
}

ImageComponent* GameObject::GetImageComponent(GameObject* go)
{
	int comp = go->SearchComponent(go, ComponentType::UI_IMAGE);

	if (comp != -1)
		return (ImageComponent*)go->components[comp];
	else
		return nullptr;
}

CheckboxComponent* GameObject::GetCheckboxComponent(GameObject* go)
{
	int comp = go->SearchComponent(go, ComponentType::UI_CHECKBOX);

	if (comp != -1)
		return (CheckboxComponent*)go->components[comp];
	else
		return nullptr;
}

InputBoxComponent* GameObject::GetInputboxComponent(GameObject* go)
{
	int comp = go->SearchComponent(go, ComponentType::UI_INPUTBOX);

	if (comp != -1)
		return (InputBoxComponent*)go->components[comp];
	else
		return nullptr;
}

SliderComponent* GameObject::GetSliderComponent(GameObject* go)
{
	int comp = go->SearchComponent(go, ComponentType::UI_SLIDER);

	if (comp != -1)
		return (SliderComponent*)go->components[comp];
	else
		return nullptr;
}

ComponentMaterial* GameObject::GetMaterialComp(GameObject* go)
{
	int comp = go->SearchComponent(go, ComponentType::MATERIAL);

	if (comp != -1)
		return (ComponentMaterial*)go->components[comp];
	else
		return nullptr;
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

std::vector<float2> GameObject::GetTexCoords(int _id)
{
	std::vector<float2> texCoords;
	for (int i = 0; i < App->renderer3D->models.size(); i++)
	{
		if (App->renderer3D->models[i].id == _id)
		{
			for (int j = 0; j < App->renderer3D->models[i].meshes.size(); j++)
			{
				for (int k = 0; k < App->renderer3D->models[i].meshes[j].vertices.size(); k++)
				{
					texCoords.push_back(App->renderer3D->models[i].meshes[j].vertices[k].texCoords);
				}
			}
		}
	}
	return texCoords;
}

std::vector<uint> GameObject::GetIndices(int _id)
{
	std::vector<uint> indices;
	for (int i = 0; i < App->renderer3D->models.size(); i++)
	{
		if (App->renderer3D->models[i].id == _id)
		{
			for (int j = 0; j < App->renderer3D->models[i].meshes.size(); j++)
			{
				for (int k = 0; k < App->renderer3D->models[i].meshes[j].indices.size(); k++)
				{
					indices.push_back(App->renderer3D->models[i].meshes[j].indices[k]);
				}
			}
		}
	}
	return indices;
}
