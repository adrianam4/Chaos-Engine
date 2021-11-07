#include "Component.h"
#include "GameObject.h"

u32 Component::GenerateUID()
{
	LCG uidGenerator;
	return uidGenerator.IntFast();
}
