#include "Component.h"
#include "GameObject.h"

Component::~Component() 
{
	
}

u32 Component::GenerateUID()
{
	LCG uidGenerator;
	return uidGenerator.IntFast();
}
