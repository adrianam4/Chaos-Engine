#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h" 
#include "CameraComponent.h"

#include<stack>
ModuleCamera3D::ModuleCamera3D(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	editorCam = new ComponentCamera(float3(0, 0, 5), 70, .01f, 100, false);
	editorCam->reference = Vec3(0.0f, 0.0f, 0.0f);
	editorCam->position = Vec3(0.0f, 0.0f, 5.0f);
	editorCam->x = Vec3(1.0f, 0.0f, 0.0f);
	editorCam->y = Vec3(0.0f, 1.0f, 0.0f);
	editorCam->z = Vec3(0.0f, 0.0f, 1.0f);
	CalculateViewMatrix(editorCam);
	
	//originCam = cam;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	App->editor->AddLog("Setting up the camera\n");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	App->editor->AddLog("Cleaning camera\n");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	Vec3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed * 2;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed * 2;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += Vec3(editorCam->frustum.front.x, editorCam->frustum.front.y, editorCam->frustum.front.z) * speed * 2;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= Vec3(editorCam->frustum.front.x, editorCam->frustum.front.y, editorCam->frustum.front.z) * speed * 2;

	if (App->input->GetMouseZ() > 0) newPos += Vec3(editorCam->frustum.front.x, editorCam->frustum.front.y, editorCam->frustum.front.z) * speed * 6;
	if (App->input->GetMouseZ() < 0) newPos -= Vec3(editorCam->frustum.front.x, editorCam->frustum.front.y, editorCam->frustum.front.z) * speed * 6;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= Vec3(editorCam->frustum.WorldRight().x, editorCam->frustum.WorldRight().y, editorCam->frustum.WorldRight().z) * speed * 2;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += Vec3(editorCam->frustum.WorldRight().x, editorCam->frustum.WorldRight().y, editorCam->frustum.WorldRight().z) * speed * 2;

	editorCam->frustum.pos = float3(editorCam->position.x += newPos.x, editorCam->position.y += newPos.y, editorCam->position.z += newPos.z);
	editorCam->position += newPos;
	editorCam->reference += newPos;

	// Mouse motion ----------------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensitivity = 0.25f;

		//PROBLEM!!!!!!!!!!
		//editorCam->RecalculateRotation((App->input->GetMouseXMotion()) * -1, (App->input->GetMouseYMotion()) * -1); //Recalculate rotation (???)
		//editorCam->frustum.SetPos(vec(editorCam->position.x -= editorCam->reference.x, editorCam->position.y -= editorCam->reference.y, editorCam->position.z -= editorCam->reference.z));

		if (dx != 0)
		{
			float deltaX = (float)dx * sensitivity;

			editorCam->x = rotate(editorCam->x, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			editorCam->y = rotate(editorCam->y, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			editorCam->z = rotate(editorCam->z, deltaX, Vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float deltaY = (float)dy * sensitivity;

			editorCam->y = rotate(editorCam->y, deltaY, editorCam->x);
			editorCam->z = rotate(editorCam->z, deltaY, editorCam->x);

			if (editorCam->y.y < 0.0f)
			{
				editorCam->z = Vec3(0.0f, editorCam->z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				editorCam->y = cross(editorCam->z, editorCam->x);
			}
		}

		//editorCam->position = editorCam->reference + editorCam->z * length(editorCam->position);
		editorCam->frustum.pos = float3(editorCam->position.x, editorCam->position.y, editorCam->position.z);

		//editorCam->position -= editorCam->reference;
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) && App->input->GetKey(SDL_SCANCODE_LALT))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensitivity = 0.25f;

		//PROBLEM!!!!!!!!!!
		editorCam->RecalculateRotation((App->input->GetMouseXMotion()) * -1, (App->input->GetMouseYMotion()) * -1); //Recalculate rotation (???)
		editorCam->frustum.pos = float3(editorCam->position.x -= editorCam->reference.x, editorCam->position.y -= editorCam->reference.y, editorCam->position.z -= editorCam->reference.z);

		if (dx != 0)
		{
			float deltaX = (float)dx * sensitivity;
			
			editorCam->x = rotate(editorCam->x, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			editorCam->y = rotate(editorCam->y, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			editorCam->z = rotate(editorCam->z, deltaX, Vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float deltaY = (float)dy * sensitivity;

			editorCam->y = rotate(editorCam->y, deltaY, editorCam->x);
			editorCam->z = rotate(editorCam->z, deltaY, editorCam->x);

			if (editorCam->y.y < 0.0f)
			{
				editorCam->z = Vec3(0.0f, editorCam->z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				editorCam->y = cross(editorCam->z, editorCam->x);
			}
		}

		editorCam->position = editorCam->reference + editorCam->z * length(editorCam->position);
		editorCam->frustum.pos = float3(editorCam->position.x, editorCam->position.y, editorCam->position.z);
	}

	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		if (App->editor->objectSelected == NULL)
		{
			Look(Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f), true);
		}
		else
		{
			bool exitFunction = false;
			for (int i = 0; i < App->editor->objectSelected->components.size(); i++)
			{
				std::string isEmpty = std::string(App->editor->objectSelected->name);
				isEmpty = isEmpty.substr(0, 5);
				if (App->editor->objectSelected->components[i]->type == ComponentType::CAMERA || "Empty" == isEmpty)
				{
					Look(Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f), true); // TODO
					exitFunction = true;
					break;
				}
			}
			if (!exitFunction)
			{
				float3 objectScale = App->editor->objectSelected->getTransform()->scale;
				float generalScale = objectScale.x * objectScale.y * objectScale.z;
				float3 objectPoition = App->editor->objectSelected->getTransform()->position;
				int aabbSize = App->editor->objectSelected->aabb.size();
				float3 Max;
				float3 Min;
				if (aabbSize > 1)
				{
					float* maxarray = new float[aabbSize];

					for (int a = 0; a < aabbSize; a++)
					{
						maxarray[a] = (App->editor->objectSelected->aabb[a]->maxPoint.x - App->editor->objectSelected->aabb[a]->minPoint.x) + (App->editor->objectSelected->aabb[a]->maxPoint.y - App->editor->objectSelected->aabb[a]->minPoint.y) + (App->editor->objectSelected->aabb[a]->maxPoint.z - App->editor->objectSelected->aabb[a]->minPoint.z);

					}
					int max = maxarray[0];
					int index = 0;
					for (int i = 1; i < aabbSize; i++)
					{
						if (maxarray[i] > max) {
							max = maxarray[i];
							index = i;
						}
					}
					Max = App->editor->objectSelected->aabb[index]->maxPoint;
					Min = App->editor->objectSelected->aabb[index]->minPoint;
					delete[] maxarray;
				}
				else
				{
					Max = App->editor->objectSelected->aabb[0]->maxPoint;
					Min = App->editor->objectSelected->aabb[0]->minPoint;
				}
				editorCam->position.y = (objectPoition.y + ((objectScale.y * (Max.y - Min.y)) / 2));
				float h = ((Max.y - Min.y) * objectScale.y) + ((Max.x - Min.x) * objectScale.x) + ((Max.z - Min.z) * objectScale.z);
				float distance = h / math::Atan(75 / 2);
				editorCam->position.z = (objectPoition.z + distance);
				editorCam->position.x = (objectPoition.x + distance);
				LookAt({ (((Max.x - Min.x) / 2) + objectPoition.x),(objectPoition.y + ((objectScale.y * (Max.y - Min.y)) / 2)),(((Max.z - Min.z) / 2) + objectPoition.z) });
				editorCam->frustum.pos = float3(editorCam->position.x, editorCam->position.y, editorCam->position.z);
			}
		}
	}

	editorCam->frustumMatrix = editorCam->frustum.ViewMatrix(); //Get CameraViewMatrix

	// Recalculate matrix -------------
	CalculateViewMatrix(editorCam);
	if (GameCam != nullptr) {
		GameCam->frustumMatrix = GameCam->frustum.ViewMatrix(); //Get CameraViewMatrix

	// Recalculate matrix -------------
		CalculateViewMatrix(GameCam);
	}

	//Test if a ray intersects with gameobjects
	if (!App->gameMode && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		float2 mousePos = { (float)App->input->GetMouseX() ,(float)App->input->GetMouseY() };
		float2 mPos = { ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y };
		float4 viewport = App->editor->viewport;

		if (mousePos.x > viewport.x && mousePos.x < viewport.x + viewport.z && mousePos.y > viewport.y && mousePos.y < viewport.y + viewport.w)
		{	
			float2 fMousePos = { mPos.x - viewport.x , mPos.y - viewport.y};
			float normalized_x = Lerp(-1, 1, fMousePos.x / viewport.z);
			float normalized_y = Lerp(1, -1, fMousePos.y / viewport.w);

			editorCam->frustum.GetCornerPoints(&corners[0]);
			myRay = editorCam->frustum.UnProjectLineSegment(normalized_x, normalized_y);

			std::stack<GameObject*>stackNode;
			GameObject* theObject;
			for (int i = 0; i < App->scene->gameObjects.size(); i++)
			{

				for (int a = 0; a < App->scene->gameObjects[i]->childrens.size(); a++) {
					stackNode.push(App->scene->gameObjects[i]->childrens[a]);
				}
				while (!stackNode.empty())
				{
					theObject = stackNode.top();
					stackNode.pop();
					/*int y = App->scene->gameObjects[i]->aabb.size();
					math::AABB* e = theObject->aabb[y - 1];*/
					GameObject* go = theObject;
					for (int j = 0; j < go->boundingBoxes.size(); j++)
					{
						if (bool hit = myRay.Intersects(*go->aabb[j]))
						{
							hitObjs.push_back(go);
						}
					}
					for (unsigned i = 0; i < theObject->childrens.size(); ++i)
					{
						stackNode.push(theObject->childrens[i]);
					}
				}
			}
			for (int i = 0; i < App->scene->gameObjects.size(); i++)
			{
				GameObject* go = App->scene->gameObjects[i];
				for (int j = 0; j < go->boundingBoxes.size(); j++)
				{
					if (bool hit = myRay.Intersects(*go->aabb[j]))
					{
						hitObjs.push_back(go);
					}
				}

			}
			if (hitObjs.size() > 0)
			{
				std::vector<float> distance;
				float nearDist = 500.0f;
				int nearObj = 0;
				for (int i = 0; i < hitObjs.size(); ++i)
				{
					int myComp = hitObjs[i]->SearchComponent(hitObjs[i], ComponentType::TRANSFORM);
					if (myComp == -1)
					{
						myComp = hitObjs[i]->SearchComponent(hitObjs[i], ComponentType::TRANSFORM2D);
					}
					float3 distnceVec = hitObjs[i]->components[myComp]->position - editorCam->frustum.pos;
					float finalDistance = math::Sqrt((distnceVec.x * distnceVec.x) + (distnceVec.y * distnceVec.y) + (distnceVec.z * distnceVec.z));
					distance.push_back(finalDistance);
				}
				for (int i = 0; i < distance.size(); i++)
				{
					if (distance[i] < nearDist)
					{
						nearDist = distance[i];
						nearObj = i;
					}
				}
				App->editor->objectSelected = hitObjs[nearObj];
				hitObjs.clear();
			}
		}
	}

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const Vec3& position, const Vec3& reference, bool rotateAroundReference)
{
	editorCam->position = position;
	editorCam->reference = reference;

	editorCam->z = normalize(position - reference);
	editorCam->x = normalize(cross(Vec3(0.0f, 1.0f, 0.0f), editorCam->z));
	editorCam->y = cross(editorCam->z, editorCam->x);

	if (!rotateAroundReference)
	{
		editorCam->reference = editorCam->position;
		editorCam->position += editorCam->z * 0.05f;
	}

	CalculateViewMatrix(editorCam);
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const Vec3& spot)
{
	editorCam->reference = spot;

	editorCam->z = normalize(editorCam->position - editorCam->reference);
	editorCam->x = normalize(cross(Vec3(0.0f, 1.0f, 0.0f), editorCam->z));
	editorCam->y = cross(editorCam->z, editorCam->x);

	CalculateViewMatrix(editorCam);
}

void ModuleCamera3D::RecalculateProjection()
{
/*	cam->frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);
	cam->frustum.SetViewPlaneDistances(nPlane, fPlane);
	float hFovR = DegToRad(hFov);
	float vFovR = 2 * Atan((Tan(hFovR / 2)) * ((float)SCREEN_HEIGHT / (float)SCREEN_WIDTH));
	cam->frustum.SetPerspective(hFovR, vFovR);
	cam->frustum.ComputeProjectionMatrix();*/
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(const Vec3& movement)
{
	editorCam->position += movement;
	editorCam->reference += movement;

	CalculateViewMatrix(editorCam);
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &viewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix(ComponentCamera* editorCam)
{
	viewMatrix = mat4x4(editorCam->x.x, editorCam->y.x, editorCam->z.x, 0.0f, editorCam->x.y, editorCam->y.y, editorCam->z.y, 0.0f, editorCam->x.z, editorCam->y.z, editorCam->z.z, 0.0f, -dot(editorCam->x, editorCam->position), -dot(editorCam->y, editorCam->position), -dot(editorCam->z, editorCam->position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}