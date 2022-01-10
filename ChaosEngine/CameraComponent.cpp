#include "Application.h"
#include "Globals.h"
#include <vector>

#include "Component.h"
#include "CameraComponent.h"

ComponentCamera::ComponentCamera(float3 pos, double hFov, double nPlane, double fPlane, bool isGameObj)
{
	type = ComponentType::CAMERA;
	UID = GenerateUID();
	name = "Camera Component";

	frustum.type = PerspectiveFrustum;
	frustum.nearPlaneDistance = nPlane;
	frustum.farPlaneDistance = fPlane;
	frustum.pos = pos;
	float hFovR = DegToRad(hFov);
	float vFovR = 2 * Atan((Tan(hFovR / 2)) * ((float)SCREEN_HEIGHT / (float)SCREEN_WIDTH));
	frustum.horizontalFov = hFovR;
	frustum.verticalFov = vFovR;

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
	//changeViewMatrix();
}
void ComponentCamera::CalculateViewMatrix()
{
	viewMatrix = mat4x4(x.x, y.x, z.x, 0.0f, x.y, y.y, z.y, 0.0f, x.z, y.z, z.z, 0.0f, -dot(x, position), -dot(y, position), -dot(z, position), 1.0f);
	
}
ComponentCamera::~ComponentCamera()
{
	if (App->camera->camArray.size() - 1 > 0) {
		for (int a = 0; a < App->camera->camArray.size(); a++)
		{
			if (App->camera->camArray[a] == this) {
				App->camera->camArray.erase(App->camera->camArray.begin() + a);
			}
			else {
				App->camera->camArray[a]->isTheMainCamera = true;
				App->camera->GameCam = App->camera->camArray[a];
			}
		}
	}
	else {
		App->camera->GameCam = nullptr;
	}
}

void ComponentCamera::Enable()
{
	App->camera->camArray.push_back(this);
	CalculatePoints();
}
void ComponentCamera::pre() {
	if (start) {
		App->viewportBuffer->enableStart(this);
		start = false;
	}
	App->viewportBuffer->pre(this);
}
void ComponentCamera::post() {
	App->viewportBuffer->post(this);
}
void ComponentCamera::Update()
{
	//if (this != App->camera->editorCam) 
	//{
		CalculatePoints();
		RecalculateCamera();
		frustum.pos = { position.x,position.y,position.z };
		frustum.up = { y.x,y.y,y.z };
		frustum.front = { z.x,z.y,z.z };
		frustum.ProjectionMatrix();
	//}
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
			
			//App->camera->cam = App->camera->originCam;
			if (App->camera->camArray.size() - 1 > 0) {
				for (int a = 0; a < App->camera->camArray.size(); a++)
				{
					if (App->camera->camArray[a] != this) {
						App->camera->camArray[a]->isTheMainCamera = true;
						App->camera->GameCam = App->camera->camArray[a];
					}
					
				}
			}
			else {
				App->camera->GameCam = nullptr;
			}
		}
		if (isTheMainCamera == true)
		{
			//App->camera->GameCam = this;
			for (int a = 0; a < App->camera->camArray.size(); a++)
			{
				if (App->camera->camArray[a] != this) {
					App->camera->camArray[a]->isTheMainCamera = false;
				}
				else {
					App->camera->camArray[a]->isTheMainCamera = true;
					App->camera->GameCam = this;
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
	//viewMatrix=perspective(horizontalFov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, nearPlaneDistance, farPlaneDistance);
	frustum.nearPlaneDistance = nearPlaneDistance;
	frustum.farPlaneDistance = farPlaneDistance;
	float hFovR = DegToRad(horizontalFov);
	float vFovR = 2 * Atan((Tan(hFovR / 2)) * size.y / size.x);
	frustum.horizontalFov = hFovR;
	frustum.verticalFov = vFovR;
	
}
mat4x4 ComponentCamera::getViewmatrix() {
	return viewMatrix;
}
void ComponentCamera::Rotate(float angle, const char* axis){
	if (axis == "Y") {
		x = rotate(x, angle, Vec3(0.0f, 1.0f, 0.0f));
		z = rotate(z, angle, Vec3(0.0f, 1.0f, 0.0f));
		RecalculateRotation(angle,0 );
	}
	else if (axis == "X") {
		y = rotate(y, angle, Vec3(1.0f, 0.0f, 0.0f));
		z = rotate(z, angle, Vec3(1.0f, 0.0f, 0.0f));
		RecalculateRotation(0, angle);
	}
	else {
		y = rotate(y, angle, Vec3(0.0f, 0.0f, 1.0f));
		x = rotate(x, angle, Vec3(0.0f, 0.0f, 1.0f));
	}
	

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
void ComponentCamera::Move(float3 newPosition) 
{
	position.x = newPosition.x;
	position.y = newPosition.y;
	position.z = newPosition.z;
	CalculateViewMatrix();
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
	if (App->editor->objectSelected != nullptr) {
		for (int i = 0; i < App->editor->objectSelected->components.size(); i++)
		{
			if (App->editor->objectSelected->components[i]->type == ComponentType::TRANSFORM)
			{
				frustum.pos = App->editor->objectSelected->components[i]->position;
				break;
			}
		}
	}
	
}

void ComponentCamera::CalculatePoints()
{
	float3 cornerPoints[8];
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
