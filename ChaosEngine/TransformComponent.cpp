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

	App->editor->AddLog("Quat -> w:%f x:%f y:%f z:%f \n",rotationQuat.w, rotationQuat.x, rotationQuat.y, rotationQuat.z);
	App->editor->AddLog("Angles -> x:%f y:%f z:%f \n",rotationEuler.x, rotationEuler.y, rotationEuler.z);
}

void ComponentTransform::Disable()
{
}

void ComponentTransform::OnEditor(int i)
{
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
	if (ImGui::DragFloat("Rotation X", &rotationEuler.x))
		Update();
	if (ImGui::DragFloat("Rotation Y", &rotationEuler.y))
		Update();
	if (ImGui::DragFloat("Rotation Z", &rotationEuler.z))
		Update();
}

Quat ComponentTransform::FromEulerToQuat(float3 eulerAngles)
{
	eulerAngles.x = math::DegToRad(eulerAngles.x);
	eulerAngles.y = math::DegToRad(eulerAngles.y);
	eulerAngles.z = math::DegToRad(eulerAngles.z);

	Quat q = q.FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);

	return q;
}

float3 ComponentTransform::FromQuatToEuler(Quat quatAngles)
{
	float3 angles;

	angles = quatAngles.ToEulerXYZ();

	angles.x = math::RadToDeg(angles.x);
	angles.y = math::RadToDeg(angles.y);
	angles.z = math::RadToDeg(angles.z);

	return angles;
}

//void ComponentTransform::CalculateTransMatrix(mat4x4 parentsMatrix, mat4x4 localMatrix)
//{
//	transMatrix = parentsMatrix * localMatrix;
//}