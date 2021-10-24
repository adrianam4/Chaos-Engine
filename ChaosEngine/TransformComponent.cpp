#include "Application.h"
#include "TransformComponent.h"
#include <math.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ComponentTransform::ComponentTransform(float3 pos, float3 sca, Quat rot)
{
	type = ComponentType::TRANSFORM;

	position = pos;
	scale = sca;
	rotationQuat = rot;

	rotationEuler = FromQuatToEuler(rotationQuat);

	name = "Transform Component";
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Enable()
{
}

void ComponentTransform::Update()
{
	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transMatrix = transMatrix.Transposed();
	App->editor->objectSelected->matrix = transMatrix.ptr();
}

void ComponentTransform::Disable()
{
}

void ComponentTransform::OnEditor(int i)
{
	rotationEuler = FromQuatToEuler(rotationQuat);

	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Position");
	if (ImGui::DragFloat("Position X", &position.x))
		Update();
	if (ImGui::DragFloat("Position Y", &position.y))
		Update();
	if (ImGui::DragFloat("Position Z", &position.z))
		Update();

	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Scale");
	if (ImGui::DragFloat("Scale X", &scale.x))
		Update();
	if (ImGui::DragFloat("Scale Y", &scale.y))
		Update();
	if (ImGui::DragFloat("Scale Z", &scale.z))
		Update();

	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Rotation");
	if (ImGui::DragFloat("Rotation X", &rotationEuler.x, 0.5f, -360.0f, 360.0f))
		Update();
	if (ImGui::DragFloat("Rotation Y", &rotationEuler.y, 0.5f, -360.0f, 360.0f))
		Update();
	if (ImGui::DragFloat("Rotation Z", &rotationEuler.z, 0.5f, -360.0f, 360.0f))
		Update();
}

Quat ComponentTransform::FromEulerToQuat(float3 eulerAngles)
{
	eulerAngles.x = math::DegToRad(eulerAngles.x);
	eulerAngles.y = math::DegToRad(eulerAngles.y);
	eulerAngles.z = math::DegToRad(eulerAngles.z);

	float cy = cos(eulerAngles.z * 0.5);
	float sy = sin(eulerAngles.z * 0.5);
	float cp = cos(eulerAngles.y * 0.5);
	float sp = sin(eulerAngles.y * 0.5);
	float cr = cos(eulerAngles.x * 0.5);
	float sr = sin(eulerAngles.x * 0.5);

	Quat q;
	q.w = cr * cp * cy + sr * sp * sy;
	q.x = sr * cp * cy - cr * sp * sy;
	q.y = cr * sp * cy + sr * cp * sy;
	q.z = cr * cp * sy - sr * sp * cy;

	return q;
}

float3 ComponentTransform::FromQuatToEuler(Quat quatAngles)
{
	float3 angles;

	//(x-axis rotation)
	double sinr_cosp = 2 * (quatAngles.w * quatAngles.x + quatAngles.y * quatAngles.z);
	double cosr_cosp = 1 - 2 * (quatAngles.x * quatAngles.x + quatAngles.y * quatAngles.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	//(y-axis rotation)
	double sinp = 2 * (quatAngles.w * quatAngles.y - quatAngles.z * quatAngles.x);
	if (std::abs(sinp) >= 1)
		angles.y = std::copysign(M_PI / 2, sinp);
	else
		angles.y = std::asin(sinp);

	//(z-axis rotation)
	double siny_cosp = 2 * (quatAngles.w * quatAngles.z + quatAngles.x * quatAngles.y);
	double cosy_cosp = 1 - 2 * (quatAngles.y * quatAngles.y + quatAngles.z * quatAngles.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	angles.x = math::RadToDeg(angles.x);
	angles.y = math::RadToDeg(angles.y);
	angles.z = math::RadToDeg(angles.z);

	return angles;
}

//void ComponentTransform::CalculateTransMatrix(mat4x4 parentsMatrix, mat4x4 localMatrix)
//{
//	transMatrix = parentsMatrix * localMatrix;
//}