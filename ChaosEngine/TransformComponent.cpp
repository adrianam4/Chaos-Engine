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
	generalScale = 1.0f;

	rotationEuler = FromQuatToEuler(rotationQuat);

	name = "Transform Component";


	GameObject* auxiliar;
	auxiliar = *App->scene->gameObjects.end()._Ptr;

	Model* auxiliar1;
	auxiliar1 = App->renderer3D->models.end()._Ptr;
	
	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transMatrix = transMatrix.Transposed();

	
	for (int b = 0; b < App->renderer3D->models[App->renderer3D->models.size() - 1].meshes.size(); b++)
	{
		AABB* aux = new AABB();
		OBB* aux1 = new OBB();
		


		aux->SetNegativeInfinity();
		
		for (int a = 0; a < App->renderer3D->models[App->renderer3D->models.size() - 1].meshes[b].vertices.size(); a++) {

			vertices_aux.push_back(App->renderer3D->models[App->renderer3D->models.size() - 1].meshes[b].vertices[a].position);

		}

		aux->Enclose((float3*)vertices_aux.data(), (size_t)vertices_aux.size());
		(*aux1)=(*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		App->scene->gameObjects[App->scene->gameObjects.size()-1]->aabb.push_back(aux);
		
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->obb.push_back(aux1);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		BoundingBoxes* auxiliarCube = new BoundingBoxes(aux->maxPoint,aux->minPoint);
		App->editor->boundingBoxes.push_back(auxiliarCube);
	}
	
	

	
	
				
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

	for (int a = 0; a < App->editor->objectSelected->childrens.size(); a++) {
		for (int b = 0; b < App->editor->objectSelected->childrens[a]->components.size(); b++) {
			if (App->editor->objectSelected->childrens[a]->components[b]->type == ComponentType::TRANSFORM)
			{
				App->editor->objectSelected->childrens[a]->components[b]->position += position - lastposition;

				App->editor->objectSelected->childrens[a]->matrix = transMatrix.ptr();
			}
		}
	}
	//obb = mesh->GetAABB();
	//obb.Transform(GetComponent<C_Transform>()->GetGlobalTransform());
	//// Generate global AABB
	//aabb.SetNegativeInfinity();
	//aabb.Enclose(obb);
}

void ComponentTransform::Disable()
{
}

void ComponentTransform::OnEditor(int i)
{
	lastposition = position;
	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Position");
	if (ImGui::DragFloat("Position X", &position.x))
		Update();
	if (ImGui::DragFloat("Position Y", &position.y))
		Update();
	if (ImGui::DragFloat("Position Z", &position.z))
		Update();

	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Scale");
	if (ImGui::DragFloat("General Scale", &generalScale))
	{
		scale.x = generalScale;
		scale.y = generalScale;
		scale.z = generalScale;
		Update();
	}
	ImGui::Spacing();
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