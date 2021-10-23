#include "Application.h"
#include "TransformComponent.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ComponentTransform::ComponentTransform(float3 pos, float3 sca, Quat rot)
{
	type = ComponentType::TRANSFORM;

	position = pos;
	scale = sca;
	rotation = rot;

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
	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotation, scale);
	transMatrix = transMatrix.Transposed();
	App->editor->objectSelected->matrix = transMatrix.ptr();
}

void ComponentTransform::Disable()
{
}

void ComponentTransform::OnEditor(int i)
{
	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Position");
	if (ImGui::InputFloat("Position X", &position.x))
		Update();
	if (ImGui::InputFloat("Position Y", &position.y))
		Update();
	if (ImGui::InputFloat("Position Z", &position.z))
		Update();

	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Scale");
	if (ImGui::InputFloat("Scale X", &scale.x))
		Update();
	if (ImGui::InputFloat("Scale Y", &scale.y))
		Update();
	if (ImGui::InputFloat("Scale Z", &scale.z))
		Update();

	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Rotation");
	if (ImGui::InputFloat("Rotation W", &rotation.w))
		Update();
	if (ImGui::InputFloat("Rotation X", &rotation.x))
		Update();
	if (ImGui::InputFloat("Rotation Y", &rotation.y))
		Update();
	if (ImGui::InputFloat("Rotation Z", &rotation.z))
		Update();
}

//void ComponentTransform::CalculateTransMatrix(mat4x4 parentsMatrix, mat4x4 localMatrix)
//{
//	transMatrix = parentsMatrix * localMatrix;
//}