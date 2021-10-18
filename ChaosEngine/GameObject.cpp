#include "GameObject.h"

Component* GameObject::CreateComponent(ComponentType type)
{
	Component* component;
	component = new Component();
	return component;
}

void GameObject::Update()
{
}
