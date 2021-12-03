#include "Component.h"
#include "GameObject.h"
#include "mmgr.h"
Component::~Component() {
	int a = 0;
}
u32 Component::GenerateUID()
{
	LCG uidGenerator;
	return uidGenerator.IntFast();
}
