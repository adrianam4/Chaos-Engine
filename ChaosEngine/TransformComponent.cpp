#include "TransformComponent.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ComponentTransform::ComponentTransform(float3 pos, float3 sca)
{
	type = ComponentType::TRANSFORM;

	position = pos;
	scale = sca;
	rotation = { 1,1,1,1 };
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

	mat4x4 parentsMatrix;
	mat4x4 localMatrix;
	//CalculateTransMatrix(parentsMatrix,localMatrix);
}

void ComponentTransform::Disable()
{
}

void ComponentTransform::OnEditor(int i)
{
	ImGui::Text("%.3f", position.x); ImGui::SameLine();
	ImGui::Text("%.3f", position.y); ImGui::SameLine();
	ImGui::Text("%.3f", position.z); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Position");
	ImGui::Text("%.3f", scale.x); ImGui::SameLine();
	ImGui::Text("%.3f", scale.y); ImGui::SameLine();
	ImGui::Text("%.3f", scale.z); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Scale");
	ImGui::Text("%.3f", rotation.x); ImGui::SameLine();
	ImGui::Text("%.3f", rotation.y); ImGui::SameLine();
	ImGui::Text("%.3f", rotation.z); ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Rotation");
}

//void ComponentTransform::CalculateTransMatrix(mat4x4 parentsMatrix, mat4x4 localMatrix)
//{
//	transMatrix = parentsMatrix * localMatrix;
//}
