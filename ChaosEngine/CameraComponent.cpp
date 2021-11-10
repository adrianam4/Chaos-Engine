#include "Application.h"

#include "Component.h"
#include "CameraComponent.h"

ComponentCamera::ComponentCamera(float3 pos, double hFov, double nPlane, double fPlane)
{
	type = ComponentType::CAMERA;
	UID = GenerateUID();
	name = "Camera Component";

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetViewPlaneDistances(nPlane, fPlane);
	frustum.SetPerspective(hFov, hFov);
	frustum.SetFrame(pos, float3(0, 0, -1), float3(0, 1, 0));

	horizontalFov = hFov;
	nearPlaneDistance = nPlane;
	farPlaneDistance = fPlane;
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Enable()
{
}

void ComponentCamera::Update()
{
	RecalculateCamera();
}

void ComponentCamera::Disable()
{
}

void ComponentCamera::OnEditor(int i)
{
	if (ImGui::SliderFloat("FOV", &horizontalFov, 55.0f, 110.0f))
	{
		frustum.verticalFov = frustum.horizontalFov = (horizontalFov * math::pi / 2) / 180; // Convert from deg to rads (All maths works with rads but user will see the info in degs)
	}
	if (ImGui::DragFloat("Near Plane Distance", &nearPlaneDistance))
	{
		frustum.nearPlaneDistance = nearPlaneDistance;
	}
	if (ImGui::DragFloat("Far Plane Distance", &farPlaneDistance))
	{
		frustum.farPlaneDistance = farPlaneDistance;
	}
}

void ComponentCamera::Draw()
{
	// TODO
}

void ComponentCamera::RecalculateFront(float degrees)
{
	// TODO
}

void ComponentCamera::RecalculateUp(float degrees)
{
	// TODO
}

void ComponentCamera::RecalculateCamera()
{
	for (int i = 0; i < App->editor->objectSelected->components.size(); i++)
	{
		if (App->editor->objectSelected->components[i]->type == ComponentType::TRANSFORM)
		{
			frustum.SetPos(App->editor->objectSelected->components[i]->position);
			break;
		}
	}
	RecalculateFront(0); // Set desired angles, 0 is WRONG, TODO
	RecalculateUp(0); // Set desired angles, 0 is WRONG, TODO
}
