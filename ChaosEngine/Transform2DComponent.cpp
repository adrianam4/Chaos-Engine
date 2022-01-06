#include "Application.h"
#include "Transform2DComponent.h"
#include <math.h>
#include"CameraComponent.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "Parson/parson.h"

ComponentTransform2D::ComponentTransform2D(GameObject* theObject, float3 pos, float3 sca, float3 rot)
{
	UID = GenerateUID();
	lastPosition = { 0,0,0 };
	lastRotation = { 0,0,0 };
	type = ComponentType::TRANSFORM2D;

	position = pos;
	scale = sca;
	rotationEuler = rot;
	generalScale = 1.0f;

	buttonWidth = scale.x * 10;
	buttonHeight = scale.z * 10;

	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();

	name = "Transform2D Component";

	ComponentType t = getComponentType();
	CreateAABB(ComponentType::MESH, theObject, true);
}
ComponentTransform2D::ComponentTransform2D(float3 pos, float3 sca, float3 rot)
{
	UID = GenerateUID();
	lastPosition = { 0,0,0 };
	lastRotation = { 0,0,0 };
	type = ComponentType::TRANSFORM2D;

	position = pos;
	scale = sca;
	rotationEuler = rot;
	generalScale = 1.0f;

	buttonWidth = scale.x * 10;
	buttonHeight = scale.z * 10;

	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	App->scene->gameObjects[App->scene->gameObjects.size() - 1]->matrix = transMatrix.ptr();

	name = "Transform2D Component";

	CreateAABB(ComponentType::PLANE, App->scene->gameObjects[App->scene->gameObjects.size() - 1], true);
}

ComponentType ComponentTransform2D::getComponentType()
{
	for (int b = 0; b < App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components.size(); b++)
	{
		if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::UI_BUTTON)
		{
			return ComponentType::UI_BUTTON;
		}
		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::UI_CHECKBOX)
		{
			return ComponentType::UI_CHECKBOX;
		}
		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::UI_INPUTBOX)
		{
			return ComponentType::UI_INPUTBOX;
		}
		else if (App->scene->gameObjects[App->scene->gameObjects.size() - 1]->components[b]->type == ComponentType::UI_SLIDER)
		{
			return ComponentType::UI_SLIDER;
		}
		
	}
}
ComponentTransform2D::~ComponentTransform2D()
{
}

void ComponentTransform2D::Enable()
{
}

void ComponentTransform2D::Update(bool releaseMouse)
{
	rotationQuat = FromEulerToQuat(rotationEuler);

	float4x4 aux;
	transMatrix = aux.FromTRS(position, rotationQuat, scale);
	transmat = transMatrix;
	transMatrix = transMatrix.Transposed();
	App->editor->objectSelected->matrix = transMatrix.ptr();

	if (releaseMouse)
	{
		UINT isCamera = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::CAMERA);

		if (isCamera == -1)
		{
			GameObject* go = App->editor->objectSelected;
			go->aabb.clear();

			CreateAABB(ComponentType::PLANE, go, false);
			
			go = nullptr;
			delete go;
		}
		else
		{
			App->camera->GameCam = (ComponentCamera*)App->editor->objectSelected->components[isCamera];
			App->camera->GameCam->Move(position);
		}
		
	}

	for (int i = 0; i < App->editor->objectSelected->boundingBoxes.size(); i++)
	{
		App->editor->objectSelected->boundingBoxes[i]->matrix = transMatrix.ptr();
	}

	//float4x4 aux1;

	/*for (int a = 0; a < App->editor->objectSelected->childrens.size(); a++) {
		for (int b = 0; b < App->editor->objectSelected->childrens[a]->components.size(); b++) {
			if (App->editor->objectSelected->childrens[a]->components[b]->type == ComponentType::TRANSFORM)
			{
				App->editor->objectSelected->childrens[a]->components[b]->scale += (scale - lastScale);
				App->editor->objectSelected->childrens[a]->components[b]->position += (position - lastPosition);
				App->editor->objectSelected->childrens[a]->components[b]->rotationEuler += (rotationEuler - lastRotation);
				App->editor->objectSelected->childrens[a]->components[b]->rotationQuat = FromEulerToQuat(App->editor->objectSelected->childrens[a]->components[b]->rotationEuler);

				App->editor->objectSelected->childrens[a]->components[b]->transmat = aux1.FromTRS(App->editor->objectSelected->childrens[a]->components[b]->position, App->editor->objectSelected->childrens[a]->components[b]->rotationQuat, App->editor->objectSelected->childrens[a]->components[b]->scale);
				App->editor->objectSelected->childrens[a]->components[b]->transmat = App->editor->objectSelected->childrens[a]->components[b]->transmat.Transposed();
				App->editor->objectSelected->childrens[a]->matrix = App->editor->objectSelected->childrens[a]->components[b]->transmat.ptr();
			}
		}
	}*/
}

void ComponentTransform2D::Disable()
{
}

void ComponentTransform2D::OnEditor(int i)
{
	lastScale = scale;
	lastPosition = position;
	lastRotation = rotationEuler;
	lastGeneralScale = generalScale;

	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Size");
	if (ImGui::DragFloat("Width", &buttonWidth, 0.5f, 0, 10000))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		scale.x = buttonWidth / 10;
		scale.z = buttonHeight / 10;
		Update(release);
	}
	if (ImGui::DragFloat("Height", &buttonHeight, 0.5f, 0, 10000))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		scale.x = buttonWidth / 10;
		scale.z = buttonHeight / 10;
		Update(release);
	}

	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Position");
	static bool wasNull = true;
	if (App->camera->GameCam != nullptr)
		wasNull = false;

	if (ImGui::DragFloat("Position X", &position.x, 0.5f))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		Update(release);
	}
	if (ImGui::DragFloat("Position Y", &position.y, 0.5f))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		Update(release);
	}
	if (ImGui::DragFloat("Position Z", &position.z, 0.5f))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		Update(release);
	}
	
	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Scale");
	if (ImGui::DragFloat("General Scale", &generalScale, 0.1f, 0.0f, 1000.0f))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		Update(release);
		scale.x = generalScale;
		scale.y = generalScale;
		
	}
	ImGui::Spacing();
	if (ImGui::DragFloat("Scale X", &scale.x, 0.1f, 0.0f, 1000.0f))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		buttonWidth = scale.x * 10;
		buttonHeight = scale.z * 10;
		Update(release);
	}
	if (ImGui::DragFloat("Scale Y", &scale.y, 0.1f, 0.0f, 1000.0f))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		Update(release);
	}
	if (ImGui::DragFloat("Scale Z", &scale.z, 0.1f, 0.0f, 1000.0f))
	{
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		buttonWidth = scale.x * 10;
		buttonHeight = scale.z * 10;
		Update(release);
	}
	
	ImGui::TextColored(ImVec4(0, 0, 255, 255), "Rotation");
	if (ImGui::DragFloat("Rotation X", &rotationEuler.x, 0.5f))
	{
		if (App->camera->GameCam == nullptr)
			wasNull = true;

		int camComponent = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::CAMERA);
		if (camComponent != -1)
		{
			App->camera->GameCam = (ComponentCamera*)App->editor->objectSelected->components[camComponent];
			App->camera->GameCam->Rotate(rotationEuler.x - lastRotation.x, "X");
			if (wasNull)
				App->camera->GameCam = nullptr;
		}
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		Update(release);
	}
	if (ImGui::DragFloat("Rotation Y", &rotationEuler.y, 0.5f))
	{
		if (App->camera->GameCam == nullptr)
			wasNull = true;

		int camComponent = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::CAMERA);
		if (camComponent != -1)
		{
			App->camera->GameCam = (ComponentCamera*)App->editor->objectSelected->components[camComponent];
			App->camera->GameCam->Rotate(rotationEuler.y - lastRotation.y, "Y");
			if (wasNull)
				App->camera->GameCam = nullptr;
		}
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		Update(release);
	}
	if (ImGui::DragFloat("Rotation Z", &rotationEuler.z, 0.5f))
	{
		if (App->camera->GameCam == nullptr)
			wasNull = true;

		int camComponent = App->editor->objectSelected->SearchComponent(App->editor->objectSelected, ComponentType::CAMERA);
		if (camComponent != -1)
		{
			App->camera->GameCam = (ComponentCamera*)App->editor->objectSelected->components[camComponent];
			App->camera->GameCam->Rotate(rotationEuler.z - lastRotation.z, "Z");
			if (wasNull)
				App->camera->GameCam = nullptr;
		}
		bool release = false;
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
			release = true;
		Update(release);
	}
}

void ComponentTransform2D::Load(const char* path)
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

	App->editor->AddLog("Loaded Transform 2D Component Data\n");

	Update(false);
}

void ComponentTransform2D::Save(const char* path)
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

	App->editor->AddLog("Saved Transform 2D Component Data\n");
}
void ComponentTransform2D::setOwner() 
{
	owner->matrix = transMatrix.ptr();
}

void ComponentTransform2D::CreateAABB(ComponentType type, GameObject* go, bool firstTime)
{
	GameObject* auxiliar = go;
	Model* model = nullptr;
	MyCube* cube = nullptr;
	MyPyramid* pyramid = nullptr;
	MyCylinder* cylinder = nullptr;
	MySphere* sphere = nullptr;
	MyPlane* plane = nullptr;

	switch (type)
	{
	case ComponentType::MESH:
		for (int i = 0; i < App->renderer3D->models.size(); i++)
		{
			if (go->id == App->renderer3D->models[i].id)
			{
				model = &App->renderer3D->models[i];
				break;
			}
		}
		break;
	case ComponentType::CUBE:
		for (int i = 0; i < App->editor->cubes.size(); i++)
		{
			if (go->id == App->editor->cubes[i]->id)
			{
				cube = App->editor->cubes[i];
				break;
			}
		}
		break;
	case ComponentType::PYRAMID:
		for (int i = 0; i < App->editor->pyramids.size(); i++)
		{
			if (go->id == App->editor->pyramids[i]->id)
			{
				pyramid = App->editor->pyramids[i];
				break;
			}
		}
		break;
	case ComponentType::SPHERE:
		for (int i = 0; i < App->editor->spheres.size(); i++)
		{
			if (go->id == App->editor->spheres[i]->id)
			{
				sphere = App->editor->spheres[i];
				break;
			}
		}
		break;
	case ComponentType::CYLINDER:
		for (int i = 0; i < App->editor->cylinders.size(); i++)
		{
			if (go->id == App->editor->cylinders[i]->id)
			{
				cylinder = App->editor->cylinders[i];
				break;
			}
		}
		break;
	case ComponentType::PLANE:
		for (int i = 0; i < App->editor->planes.size(); i++)
		{
			if (go->id == App->editor->planes[i]->id)
			{
				plane = App->editor->planes[i];
				break;
			}
		}
		break;
	default:
		break;
	}

	float4x4 aux2;
	transMatrix = aux2.FromTRS(position, rotationQuat, scale);
	transMatrix = transMatrix.Transposed();

	AABB* aux;
	OBB* aux1;
	BoundingBoxes* auxiliarCube;

	std::vector<float4> aabbVertexAux;
	std::vector<float3> aabbVertex;
	float3 MAX;
	float3 MIN;
	switch (type)
	{
	case ComponentType::MESH:
		for (int b = 0; b < model->meshes.size(); b++)
		{
			aux = new AABB();
			aux1 = new OBB();
			aux->SetNegativeInfinity();
			for (int a = 0; a < model->meshes[b].vertices.size(); a++)
			{
				vertices_aux.push_back(model->meshes[b].vertices[a].position);
			}
			for (int i = 0; i < vertices_aux.size(); i++)
			{
				float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
				vertex = vertex * transMatrix;
				aabbVertexAux.push_back(vertex);
			}
			for (int i = 0; i < aabbVertexAux.size(); i++)
			{
				float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
				aabbVertex.push_back(vertex);
			}
			aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
			(*aux1) = (*aux);
			aux1->Transform(transMatrix);
			aux->SetNegativeInfinity();
			aux->Enclose(*aux1);
			go->aabb.push_back(aux);
			go->obb.push_back(aux1);
			if (firstTime)
			{
				auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
				go->boundingBoxes.push_back(auxiliarCube);
			}
		}
		break;
	case ComponentType::CUBE:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = cube->getVertex();
		for (int i = 0; i < vertices_aux.size(); i++)
		{
			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
			vertex = vertex * transMatrix;
			aabbVertexAux.push_back(vertex);
		}
		for (int i = 0; i < aabbVertexAux.size(); i++)
		{
			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
			aabbVertex.push_back(vertex);
		}
		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		go->aabb.push_back(aux);
		go->obb.push_back(aux1);
		if (firstTime)
		{
			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
			go->boundingBoxes.push_back(auxiliarCube);
		}
		break;
	case ComponentType::PYRAMID:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = pyramid->getVertex();
		for (int i = 0; i < vertices_aux.size(); i++)
		{
			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
			vertex = vertex * transMatrix;
			aabbVertexAux.push_back(vertex);
		}
		for (int i = 0; i < aabbVertexAux.size(); i++)
		{
			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
			aabbVertex.push_back(vertex);
		}
		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		go->aabb.push_back(aux);
		go->obb.push_back(aux1);
		if (firstTime)
		{
			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
			go->boundingBoxes.push_back(auxiliarCube);
		}
		break;
	case ComponentType::SPHERE:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = sphere->getVertex();
		for (int i = 0; i < vertices_aux.size(); i++)
		{
			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
			vertex = vertex * transMatrix;
			aabbVertexAux.push_back(vertex);
		}
		for (int i = 0; i < aabbVertexAux.size(); i++)
		{
			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
			aabbVertex.push_back(vertex);
		}
		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		go->aabb.push_back(aux);
		go->obb.push_back(aux1);
		if (firstTime)
		{
			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
			go->boundingBoxes.push_back(auxiliarCube);
		}
		break;
	case ComponentType::CYLINDER:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = cylinder->getVertex();
		for (int i = 0; i < vertices_aux.size(); i++)
		{
			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
			vertex = vertex * transMatrix;
			aabbVertexAux.push_back(vertex);
		}
		for (int i = 0; i < aabbVertexAux.size(); i++)
		{
			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
			aabbVertex.push_back(vertex);
		}
		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		go->aabb.push_back(aux);
		go->obb.push_back(aux1);
		if (firstTime)
		{
			
			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
			go->boundingBoxes.push_back(auxiliarCube);
		}
		break;
	case ComponentType::PLANE:
		aux = new AABB();
		aux1 = new OBB();
		aux->SetNegativeInfinity();
		vertices_aux = plane->getVertex();
		for (int i = 0; i < vertices_aux.size(); i++)
		{
			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
			vertex = vertex * transMatrix;
			aabbVertexAux.push_back(vertex);
		}
		for (int i = 0; i < aabbVertexAux.size(); i++)
		{
			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
			aabbVertex.push_back(vertex);
		}
		
		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
		MAX = aux->maxPoint;
		MIN = aux->minPoint;
		(*aux1) = (*aux);
		aux1->Transform(transMatrix);
		aux->SetNegativeInfinity();
		aux->Enclose(*aux1);
		
		go->aabb.push_back(aux);
		go->obb.push_back(aux1);

		aux->maxPoint= MAX;
		aux->minPoint= MIN;
		if (firstTime)
		{
			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, MAX, MIN);
			
			go->boundingBoxes.push_back(auxiliarCube);
		}
		break;
	default:
		break;
	}
}

Quat ComponentTransform2D::FromEulerToQuat(float3 eulerAngles)
{
	eulerAngles.x = math::DegToRad(eulerAngles.x);
	eulerAngles.y = math::DegToRad(eulerAngles.y);
	eulerAngles.z = math::DegToRad(eulerAngles.z);

	Quat q = q.FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);

	return q;
}

float3 ComponentTransform2D::FromQuatToEuler(Quat quatAngles)
{
	float3 angles;

	angles = quatAngles.ToEulerXYZ();

	angles.x = math::RadToDeg(angles.x);
	angles.y = math::RadToDeg(angles.y);
	angles.z = math::RadToDeg(angles.z);
	float3 toReturn(angles.x,angles.y,angles.z);

	return toReturn;
}

//void ComponentTransform2D::CreateAABB(ComponentType type, GameObject* go, bool firstTime)
//{
//	GameObject* auxiliar = go;
//	Model* model = nullptr;
//	MyCube* cube = nullptr;
//	MyPyramid* pyramid = nullptr;
//	MyCylinder* cylinder = nullptr;
//	MySphere* sphere = nullptr;
//	MyPlane* plane = nullptr;
//
//	switch (type)
//	{
//	case ComponentType::MESH:
//		for (int i = 0; i < App->renderer3D->models.size(); i++)
//		{
//			if (go->id == App->renderer3D->models[i].id)
//			{
//				model = &App->renderer3D->models[i];
//				break;
//			}
//		}
//		break;
//	case ComponentType::CUBE:
//		for (int i = 0; i < App->editor->cubes.size(); i++)
//		{
//			if (go->id == App->editor->cubes[i]->id)
//			{
//				cube = App->editor->cubes[i];
//				break;
//			}
//		}
//		break;
//	case ComponentType::PYRAMID:
//		for (int i = 0; i < App->editor->pyramids.size(); i++)
//		{
//			if (go->id == App->editor->pyramids[i]->id)
//			{
//				pyramid = App->editor->pyramids[i];
//				break;
//			}
//		}
//		break;
//	case ComponentType::SPHERE:
//		for (int i = 0; i < App->editor->spheres.size(); i++)
//		{
//			if (go->id == App->editor->spheres[i]->id)
//			{
//				sphere = App->editor->spheres[i];
//				break;
//			}
//		}
//		break;
//	case ComponentType::CYLINDER:
//		for (int i = 0; i < App->editor->cylinders.size(); i++)
//		{
//			if (go->id == App->editor->cylinders[i]->id)
//			{
//				cylinder = App->editor->cylinders[i];
//				break;
//			}
//		}
//		break;
//	case ComponentType::PLANE:
//		for (int i = 0; i < App->editor->planes.size(); i++)
//		{
//			if (go->id == App->editor->planes[i]->id)
//			{
//				plane = App->editor->planes[i];
//				break;
//			}
//		}
//		break;
//	default:
//		break;
//	}
//
//	float3x3 aux2;
//
//
//	transMatrix = aux2.FromRS(rotationQuat, float3(scale.x, scale.y, 0));
//	transMatrix = transMatrix.Transposed();
//
//	AABB* aux;
//	OBB* aux1;
//	BoundingBoxes* auxiliarCube;
//
//	std::vector<float4> aabbVertexAux;
//	std::vector<float3> aabbVertex;
//	switch (type)
//	{
//	case ComponentType::MESH:
//		for (int b = 0; b < model->meshes.size(); b++)
//		{
//			aux = new AABB();
//			aux1 = new OBB();
//			aux->SetNegativeInfinity();
//			for (int a = 0; a < model->meshes[b].vertices.size(); a++)
//			{
//				vertices_aux.push_back(model->meshes[b].vertices[a].position);
//			}
//			for (int i = 0; i < vertices_aux.size(); i++)
//			{
//				float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
//				vertex = vertex * transMatrix;
//				aabbVertexAux.push_back(vertex);
//			}
//			for (int i = 0; i < aabbVertexAux.size(); i++)
//			{
//				float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
//				aabbVertex.push_back(vertex);
//			}
//			aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
//			(*aux1) = (*aux);
//			aux1->Transform(transMatrix);
//			aux->SetNegativeInfinity();
//			aux->Enclose(*aux1);
//			go->aabb.push_back(aux);
//			go->obb.push_back(aux1);
//			if (firstTime)
//			{
//				auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
//				go->boundingBoxes.push_back(auxiliarCube);
//			}
//		}
//		break;
//	case ComponentType::CUBE:
//		aux = new AABB();
//		aux1 = new OBB();
//		aux->SetNegativeInfinity();
//		vertices_aux = cube->getVertex();
//		for (int i = 0; i < vertices_aux.size(); i++)
//		{
//			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
//			vertex = vertex * transMatrix;
//			aabbVertexAux.push_back(vertex);
//		}
//		for (int i = 0; i < aabbVertexAux.size(); i++)
//		{
//			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
//			aabbVertex.push_back(vertex);
//		}
//		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
//		(*aux1) = (*aux);
//		aux1->Transform(transMatrix);
//		aux->SetNegativeInfinity();
//		aux->Enclose(*aux1);
//		go->aabb.push_back(aux);
//		go->obb.push_back(aux1);
//		if (firstTime)
//		{
//			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
//			go->boundingBoxes.push_back(auxiliarCube);
//		}
//		break;
//	case ComponentType::PYRAMID:
//		aux = new AABB();
//		aux1 = new OBB();
//		aux->SetNegativeInfinity();
//		vertices_aux = pyramid->getVertex();
//		for (int i = 0; i < vertices_aux.size(); i++)
//		{
//			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
//			vertex = vertex * transMatrix;
//			aabbVertexAux.push_back(vertex);
//		}
//		for (int i = 0; i < aabbVertexAux.size(); i++)
//		{
//			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
//			aabbVertex.push_back(vertex);
//		}
//		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
//		(*aux1) = (*aux);
//		aux1->Transform(transMatrix);
//		aux->SetNegativeInfinity();
//		aux->Enclose(*aux1);
//		go->aabb.push_back(aux);
//		go->obb.push_back(aux1);
//		if (firstTime)
//		{
//			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
//			go->boundingBoxes.push_back(auxiliarCube);
//		}
//		break;
//	case ComponentType::SPHERE:
//		aux = new AABB();
//		aux1 = new OBB();
//		aux->SetNegativeInfinity();
//		vertices_aux = sphere->getVertex();
//		for (int i = 0; i < vertices_aux.size(); i++)
//		{
//			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
//			vertex = vertex * transMatrix;
//			aabbVertexAux.push_back(vertex);
//		}
//		for (int i = 0; i < aabbVertexAux.size(); i++)
//		{
//			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
//			aabbVertex.push_back(vertex);
//		}
//		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
//		(*aux1) = (*aux);
//		aux1->Transform(transMatrix);
//		aux->SetNegativeInfinity();
//		aux->Enclose(*aux1);
//		go->aabb.push_back(aux);
//		go->obb.push_back(aux1);
//		if (firstTime)
//		{
//			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
//			go->boundingBoxes.push_back(auxiliarCube);
//		}
//		break;
//	case ComponentType::CYLINDER:
//		aux = new AABB();
//		aux1 = new OBB();
//		aux->SetNegativeInfinity();
//		vertices_aux = cylinder->getVertex();
//		for (int i = 0; i < vertices_aux.size(); i++)
//		{
//			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
//			vertex = vertex * transMatrix;
//			aabbVertexAux.push_back(vertex);
//		}
//		for (int i = 0; i < aabbVertexAux.size(); i++)
//		{
//			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
//			aabbVertex.push_back(vertex);
//		}
//		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
//		(*aux1) = (*aux);
//		aux1->Transform(transMatrix);
//		aux->SetNegativeInfinity();
//		aux->Enclose(*aux1);
//		go->aabb.push_back(aux);
//		go->obb.push_back(aux1);
//		if (firstTime)
//		{
//			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
//			go->boundingBoxes.push_back(auxiliarCube);
//		}
//		break;
//	case ComponentType::PLANE:
//		aux = new AABB();
//		aux1 = new OBB();
//		aux->SetNegativeInfinity();
//		vertices_aux = plane->getVertex();
//		for (int i = 0; i < vertices_aux.size(); i++)
//		{
//			float4 vertex(vertices_aux[i].x, vertices_aux[i].y, vertices_aux[i].z, 1);
//			vertex = vertex * transMatrix;
//			aabbVertexAux.push_back(vertex);
//		}
//		for (int i = 0; i < aabbVertexAux.size(); i++)
//		{
//			float3 vertex(aabbVertexAux[i].x, aabbVertexAux[i].y, aabbVertexAux[i].z);
//			aabbVertex.push_back(vertex);
//		}
//		aux->Enclose((float3*)aabbVertex.data(), (size_t)aabbVertex.size());
//		(*aux1) = (*aux);
//		aux1->Transform(transMatrix);
//		aux->SetNegativeInfinity();
//		aux->Enclose(*aux1);
//		go->aabb.push_back(aux);
//		go->obb.push_back(aux1);
//		if (firstTime)
//		{
//			auxiliarCube = new BoundingBoxes({ 0,0,0 }, scale, aux->maxPoint, aux->minPoint);
//			go->boundingBoxes.push_back(auxiliarCube);
//		}
//		break;
//	default:
//		break;
//	}
//}