#include "Application.h"
#include "MaterialComponent.h"

ComponentMaterial::ComponentMaterial(ComponentType _type, const char* _path)
{
	type = _type;
	name = "Material Component";
	texturePath = _path;
}

ComponentMaterial::~ComponentMaterial()
{
}

void ComponentMaterial::Enable()
{
}

void ComponentMaterial::Update()
{
}

void ComponentMaterial::Disable()
{
}

void ComponentMaterial::OnEditor(int i)
{

}