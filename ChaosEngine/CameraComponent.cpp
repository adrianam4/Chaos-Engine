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

	frontRotation = upRotation = 0;

	Enable();
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Enable()
{
	CalculatePoints();
}

void ComponentCamera::Update()
{
	CalculatePoints();
	RecalculateCamera();

	Draw();
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
	if (ImGui::DragFloat("Front Rotation",&frontRotation, 0.2f))
	{
		RecalculateFront(frontRotation);
	}
	if (ImGui::DragFloat("Up Rotation", &upRotation, 0.2f))
	{
		RecalculateUp(upRotation);
	}

	Update();

}

void ComponentCamera::Draw()
{
	glLineWidth(3.0f);

	glColor4f(255, 0, 0, 255);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);

	//Buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//Draw
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);

	//UnBind Buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Disable states
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentCamera::RecalculateFront(float degrees)
{
	degrees = math::DegToRad(degrees);
	Quat rot = math::Quat::RotateY(degrees);
	float3 auxFront = rot * frustum.front;
	float3 auxUp = rot * frustum.up;
	float3::Orthonormalize(auxFront, auxUp);
	frustum.front = auxFront.Normalized();
	frustum.up = auxUp.Normalized();
}

void ComponentCamera::RecalculateUp(float degrees)
{
	degrees = math::DegToRad(degrees);
	Quat rot = math::Quat::RotateAxisAngle(frustum.WorldRight(), degrees);
	float3 auxFront = rot * frustum.front;
	float3 auxUp = rot * frustum.up;
	float3::Orthonormalize(auxFront, auxUp);
	frustum.front = auxFront.Normalized();
	frustum.up = auxUp.Normalized();
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
}

void ComponentCamera::CalculatePoints()
{
	vec cornerPoints[8];
	frustum.GetCornerPoints(cornerPoints);

	vertices.clear();
	vertices.push_back(cornerPoints[0]);vertices.push_back(cornerPoints[1]);vertices.push_back(cornerPoints[2]);vertices.push_back(cornerPoints[3]);
	vertices.push_back(cornerPoints[4]);vertices.push_back(cornerPoints[5]);vertices.push_back(cornerPoints[6]);vertices.push_back(cornerPoints[7]);

	static bool recalculate = true;
	if (recalculate)
	{
		indices.push_back(3); indices.push_back(2); indices.push_back(0); indices.push_back(1);
		indices.push_back(4); indices.push_back(5); indices.push_back(7); indices.push_back(6);
		indices.push_back(2); indices.push_back(6); indices.push_back(4); indices.push_back(0);
		indices.push_back(7); indices.push_back(3); indices.push_back(1); indices.push_back(5);
		indices.push_back(3); indices.push_back(2); indices.push_back(6); indices.push_back(7);
		indices.push_back(1); indices.push_back(0); indices.push_back(4); indices.push_back(5);

		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		recalculate = false;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float3), vertices.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}
