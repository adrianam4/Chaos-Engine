#include "Application.h"
#include "Globals.h"

#include "Component.h"
#include "CameraComponent.h"

ComponentCamera::ComponentCamera(float3 pos, double hFov, double nPlane, double fPlane, bool isGameObj)
{
	type = ComponentType::CAMERA;
	UID = GenerateUID();
	name = "Camera Component";

	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	frustum.SetViewPlaneDistances(nPlane, fPlane);
	float hFovR = DegToRad(hFov);
	float vFovR = 2 * Atan((Tan(hFovR / 2)) * ((float)SCREEN_HEIGHT / (float)SCREEN_WIDTH));
	frustum.SetPerspective(hFovR, vFovR);
	frustum.SetFrame(pos, float3(0, 0, -1), float3(0, 1, 0));
	frustum.ComputeProjectionMatrix();
	reference = Vec3(0.0f, 0.0f, 0.0f);
	horizontalFov = hFov;
	nearPlaneDistance = nPlane;
	farPlaneDistance = fPlane;
	position = Vec3(0.0f, 0.0f, 5.0f);
	frontRotation = upRotation = 0;
	isTheMainCamera = false;
	if (isGameObj)
	{
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->id = App->editor->lastId + 1;
		App->editor->lastId++;

		Enable();
	}
	x = Vec3(1.0f, 0.0f, 0.0f);
	y = Vec3(0.0f, 1.0f, 0.0f);
	z = Vec3(0.0f, 0.0f, 1.0f);
	changeViewMatrix();
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::Enable()
{
	App->camera->camArray.push_back(this);
	CalculatePoints();
}

void ComponentCamera::Update()
{
	CalculatePoints();
	RecalculateCamera();
}

void ComponentCamera::Disable()
{
}

void ComponentCamera::OnEditor(int i)
{
	if (ImGui::Checkbox("Is The Main Camera", &isTheMainCamera))
	{
		
		
		if (isTheMainCamera == false) 
		{
			
			App->camera->cam = App->camera->originCam;
			
		}
		if (isTheMainCamera == true)
		{
			
			for (int a = 0; a < App->camera->camArray.size(); a++)
			{
				if (App->camera->camArray[a] != this) {
					App->camera->camArray[a]->isTheMainCamera = false;
				}
				else {
					App->camera->camArray[a]->isTheMainCamera = true;
					App->camera->cam = this;
				}
			}
			

		}
		
	}
	if (ImGui::SliderFloat("FOV", &horizontalFov, 55.0f, 110.0f))
	{
		frustum.horizontalFov = math::DegToRad(horizontalFov); // Convert from deg to rads (All maths works with rads but user will see the info in degs)
		frustum.verticalFov = 2 * Atan((Tan(frustum.horizontalFov / 2)) * ((float)SCREEN_HEIGHT / (float)SCREEN_WIDTH));
		changeViewMatrix();
		App->renderer3D->OnResize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	}
	if (ImGui::DragFloat("Near Plane Distance", &nearPlaneDistance, 0.5f, 1, 5))
	{
		frustum.nearPlaneDistance = nearPlaneDistance;
		changeViewMatrix();
		App->renderer3D->OnResize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	}
	if (ImGui::DragFloat("Far Plane Distance", &farPlaneDistance, 0.5f, 4, 500))
	{
		frustum.farPlaneDistance = farPlaneDistance;
		changeViewMatrix();
		App->renderer3D->OnResize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
	}
	
}
void ComponentCamera::changeViewMatrix() {
	viewMatrix=perspective(horizontalFov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlaneDistance, farPlaneDistance);
	
	
}
mat4x4 ComponentCamera::getViewmatrix() {
	return viewMatrix;
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

void ComponentCamera::RecalculateRotation(float xDegrees, float yDegrees)
{
	xDegrees = math::DegToRad(xDegrees);
	Quat rot1 = math::Quat::RotateY(xDegrees);
	yDegrees = math::DegToRad(yDegrees);
	Quat rot2 = math::Quat::RotateAxisAngle(frustum.WorldRight(), yDegrees);
	Quat rot = rot1 * rot2;

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
