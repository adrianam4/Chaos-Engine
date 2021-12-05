#include "Application.h"
#include "Resource.h"

Resource::Resource()
{
}

Resource::Resource(u32 _UID, ResourceType _type)
{
	UID = _UID;
	type = _type;
}

Resource::~Resource()
{
}