#include "Application.h"
#include "TransformComponent.h"
#include <math.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "Parson/parson.h"

ComponentTransform::ComponentTransform(float3 pos, float3 sca, Quat rot)
{
	UID = GenerateUID();
	lastPosition = { 0,0,0 };
	lastRotation = {0,0,0};
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
	
	float4x4 aux2;
	transMatrix = aux2.FromTRS(position, rotationQuat, scale);
	transMatrix = transMatrix.Transposed();
	
		
	AABB* aux;
	OBB* aux1;
	BoundingBoxes* auxiliarCube;
	ComponentType t = getComponentType();
	switch (t)
	{
	case ComponentType::MESH:
		for (int b = 0; b < App->renderer3D->models[App->renderer3D->models.size() - 1].meshes.size(); b++)
		{
			aux = new AABB();
			aux1 = new OBB();
			aux->SetNegativeInfinity();
			for (int a = 0; a < App->renderer3D->models[App->renderer3D->models.size() - 1].meshes[b].vertices.size(); a++) {

				vertices_aux.push_back(App->renderer3D->models[App->renderer3D->models.size() - 1].meshes[b].vertices[a].position);

			}
			aux->Enclose((float3*)vertices_aux.data(), (size_t)vertices_aux.size());
			(*aux1) = (*aux);
			aux1->Transform(transMatrix);
			aux->SetNegativeInfinity();
			aux->Enclose(*aux1);
			App->scene->gameObjects[App->scene->gameObjects.size() - 1]->aabb.push_back(aux);

			App->scene->gameObjects[App->scene->gameObjects.size() - 1]->obb.push_back(aux1);
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			auxiliarCube = new BoundingBoxes({0,0,0}, scale, aux->maxPoint, aux->minPoint);
			App->scene->gameObjects[App->scene->gameObjects.size() - 1]->boundingBoxes.push_back(auxiliarCube);
		}

		break;
	case ComponentType::CUBE:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = App->editor->cubes[App->editor->cubes.size() - 1]->getVertex();
		aux->Enclose((float3*)vertices_aux.data(), (size_t)vertices_aux.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->aabb.push_back(aux);

		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->obb.push_back(aux1);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auxiliarCube = new BoundingBoxes({0,0,0}, scale, aux->maxPoint, aux->minPoint);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->boundingBoxes.push_back(auxiliarCube);
		break;
	case ComponentType::PYRAMID:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = App->editor->pyramids[App->editor->pyramids.size() - 1]->getVertex();
		aux->Enclose((float3*)vertices_aux.data(), (size_t)vertices_aux.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->aabb.push_back(aux);

		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->obb.push_back(aux1);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->boundingBoxes.push_back(auxiliarCube);
		break;
	case ComponentType::SPHERE:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = App->editor->spheres[App->editor->spheres.size() - 1]->getVertex();
		aux->Enclose((float3*)vertices_aux.data(), (size_t)vertices_aux.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->aabb.push_back(aux);

		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->obb.push_back(aux1);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->boundingBoxes.push_back(auxiliarCube);
		break;
	case ComponentType::CYLINDER:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = App->editor->cylinders[App->editor->cylinders.size() - 1]->getVertex();
		aux->Enclose((float3*)vertices_aux.data(), (size_t)vertices_aux.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->aabb.push_back(aux);

		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->obb.push_back(aux1);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->boundingBoxes.push_back(auxiliarCube);
		break;
	case ComponentType::PLANE:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = App->editor->planes[App->editor->planes.size() - 1]->getVertex();
		aux->Enclose((float3*)vertices_aux.data(), (size_t)vertices_aux.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->aabb.push_back(aux);

		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->obb.push_back(aux1);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
		App->scene->gameObjects[App->scene->gameObjects.size() - 1]->boundingBoxes.push_back(auxiliarCube);
		break;
	default:
		break;
	}			
}
ComponentType ComponentTransform::getComponentType() 
{
	
	for (int b = 0; b < App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components.size(); b++) {
		if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::MESH) {
			return ComponentType::MESH;
		}else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::CUBE) {
			return ComponentType::CUBE;
		}else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::PYRAMID) {
			return ComponentType::PYRAMID;

		}
		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::SPHERE) {

			return ComponentType::SPHERE;
		}
		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::CYLINDER) {
			return ComponentType::CYLINDER;

		}
		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::PLANE) {
			return ComponentType::PLANE;

		}
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

	for (int i = 0; i < App->editor->objectSelected->boundingBoxes.size(); i++)
	{
		App->editor->objectSelected->boundingBoxes[i]->matrix = transMatrix.ptr();
	}
	
	float4x4 aux1;
	
	for (int a = 0; a < App->editor->objectSelected->childrens.size(); a++) {
		for (int b = 0; b < App->editor->objectSelected->childrens[a]->components.size(); b++) {
			if (App->editor->objectSelected->childrens[a]->components[b]->type == ComponentType::TRANSFORM)
			{
				//App->editor->objectSelected->childrens[a]->components[b]->position += position;
				App->editor->objectSelected->childrens[a]->components[b]->scale += (scale - lastScale);
				App->editor->objectSelected->childrens[a]->components[b]->position += (position - lastPosition);
				App->editor->objectSelected->childrens[a]->components[b]->rotationEuler += (rotationEuler - lastRotation);
				App->editor->objectSelected->childrens[a]->components[b]->rotationQuat = FromEulerToQuat(App->editor->objectSelected->childrens[a]->components[b]->rotationEuler);

				App->editor->objectSelected->childrens[a]->components[b]->transmat = aux1.FromTRS(App->editor->objectSelected->childrens[a]->components[b]->position, App->editor->objectSelected->childrens[a]->components[b]->rotationQuat, App->editor->objectSelected->childrens[a]->components[b]->scale);
				App->editor->objectSelected->childrens[a]->components[b]->transmat = App->editor->objectSelected->childrens[a]->components[b]->transmat.Transposed();
				App->editor->objectSelected->childrens[a]->matrix = App->editor->objectSelected->childrens[a]->components[b]->transmat.ptr();

							
				//App->editor->objectSelected->childrens[a]->matrix = transMatrix.ptr();
			}
		}
	}
}

void ComponentTransform::Disable()
{
}

void ComponentTransform::OnEditor(int i)
{
	if (ImGui::Button("Save"))
		Save("Settings/TransformComponent.json");
	ImGui::SameLine();
	if (ImGui::Button("Load"))
		Load("Settings/TransformComponent.json");

	lastScale = scale;
	lastPosition = position;
	lastRotation = rotationEuler;
	lastGeneralScale = generalScale;
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

void ComponentTransform::Load(const char* path)
{
	//Reading JSON File
	JSON_Value* userData = json_parse_file(path);

	UID = json_object_get_number(json_object(userData), "UID");
	position.x = json_object_get_number(json_object(userData), "Position X");
	position.y = json_object_get_number(json_object(userData), "Position Y");
	position.z = json_object_get_number(json_object(userData), "Position Z");
	scale.x = json_object_get_number(json_object(userData), "Scale X");
	scale.y = json_object_get_number(json_object(userData), "Scale Y");
	scale.z = json_object_get_number(json_object(userData), "Scale Z");
	rotationEuler.x = json_object_get_number(json_object(userData), "Rotation X");
	rotationEuler.y = json_object_get_number(json_object(userData), "Rotation Y");
	rotationEuler.z = json_object_get_number(json_object(userData), "Rotation Z");

	App->editor->AddLog("Loaded Transform Component Data\n");

	Update();
}

void ComponentTransform::Save(const char* path)
{
	//Creating Json file
	JSON_Value* user_data = json_value_init_object();

	json_object_set_number(json_object(user_data), "UID", UID);
	json_object_set_number(json_object(user_data), "Position X", position.x);
	json_object_set_number(json_object(user_data), "Position Y", position.y);
	json_object_set_number(json_object(user_data), "Position Z", position.z);
	json_object_set_number(json_object(user_data), "Scale X", scale.x);
	json_object_set_number(json_object(user_data), "Scale Y", scale.y);
	json_object_set_number(json_object(user_data), "Scale Z", scale.z);
	json_object_set_number(json_object(user_data), "Rotation X", rotationEuler.x);
	json_object_set_number(json_object(user_data), "Rotation Y", rotationEuler.y);
	json_object_set_number(json_object(user_data), "Rotation Z", rotationEuler.z);

	json_serialize_to_file_pretty(user_data, path);

	App->editor->AddLog("Saved Transform Component Data\n");
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