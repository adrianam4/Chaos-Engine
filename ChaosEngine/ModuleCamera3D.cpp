#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"
#include "CameraComponent.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	cam = new ComponentCamera(float3(0, 0, 5), 75, .01f, 100, false);
	cam->reference = Vec3(0.0f, 0.0f, 0.0f);
	cam->position = Vec3(0.0f, 0.0f, 5.0f);
	cam->x = Vec3(1.0f, 0.0f, 0.0f);
	cam->y = Vec3(0.0f, 1.0f, 0.0f);
	cam->z = Vec3(0.0f, 0.0f, 1.0f);
	CalculateViewMatrix(cam);
	
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

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += Vec3(cam->frustum.front.x, cam->frustum.front.y, cam->frustum.front.z) * speed * 2;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= Vec3(cam->frustum.front.x, cam->frustum.front.y, cam->frustum.front.z) * speed * 2;

	if (App->input->GetMouseZ() > 0) newPos += Vec3(cam->frustum.front.x, cam->frustum.front.y, cam->frustum.front.z) * speed * 6;
	if (App->input->GetMouseZ() < 0) newPos -= Vec3(cam->frustum.front.x, cam->frustum.front.y, cam->frustum.front.z) * speed * 6;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= Vec3(cam->frustum.WorldRight().x, cam->frustum.WorldRight().y, cam->frustum.WorldRight().z) * speed * 2;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += Vec3(cam->frustum.WorldRight().x, cam->frustum.WorldRight().y, cam->frustum.WorldRight().z) * speed * 2;

	cam->frustum.SetPos(vec(cam->position.x += newPos.x, cam->position.y += newPos.y, cam->position.z += newPos.z));
	cam->reference += newPos;

	// Mouse motion ----------------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT || (App->input->GetMouseButton(SDL_BUTTON_LEFT) && App->input->GetKey(SDL_SCANCODE_LALT)))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensitivity = 0.25f;

		cam->RecalculateRotation((App->input->GetMouseXMotion()) * -1, (App->input->GetMouseYMotion()) * -1); //Recalculate rotation (???)

		//position -= reference;
		cam->frustum.SetPos(vec(cam->position.x -= cam->reference.x, cam->position.y -= cam->reference.y, cam->position.z -= cam->reference.z));

		if (dx != 0)
		{
			float deltaX = (float)dx * sensitivity;

			cam->x = rotate(cam->x, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			cam->y = rotate(cam->y, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			cam->z = rotate(cam->z, deltaX, Vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float deltaY = (float)dy * sensitivity;

			cam->y = rotate(cam->y, deltaY, cam->x);
			cam->z = rotate(cam->z, deltaY, cam->x);

			if (cam->y.y < 0.0f)
			{
				cam->z = Vec3(0.0f, cam->z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				cam->y = cross(cam->z, cam->x);
			}
		}

		cam->position = cam->reference + cam->z * length(cam->position);
		cam->frustum.SetPos(vec(cam->position.x, cam->position.y, cam->position.z));
	}
	glBegin(GL_LINES);	
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(myRay.a.x,myRay.a.y,myRay.a.z);
	glColor3f(0.f, 0.f, 0.f);
	glVertex3f(myRay.b.x, myRay.b.y, myRay.b.z);
	glColor3f(1.f, 1.f, 1.f);
	glEnd();

	//Tienes el frustrum mal formado y por eso los rayos no estan funcionando. Haz click en la pantalla y luego alejate pulsando S
	// veras como tienes el frustrum como torcido hacia arriba - El centro de la X formada por el frustrum debería estar en el centro de la pantalla
	// La parte del rayo parece correcta mira a ver que esta pasando con el frustum 

	glBegin(GL_LINES);

	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(corners[0].x, corners[0].y, corners[0].z);
	glColor3f(0.f, 0.f, 0.f);
	glVertex3f(corners[4].x, corners[4].y, corners[4].z);

	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(corners[1].x, corners[1].y, corners[1].z);
	glColor3f(0.f, 0.f, 0.f);
	glVertex3f(corners[5].x, corners[5].y, corners[5].z);

	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(corners[2].x, corners[2].y, corners[2].z);
	glColor3f(0.f, 0.f, 0.f);
	glVertex3f(corners[6].x, corners[6].y, corners[6].z);

	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(corners[3].x, corners[3].y, corners[3].z);
	glColor3f(0.f, 0.f, 0.f);
	glVertex3f(corners[7].x, corners[7].y, corners[7].z);

	glColor3f(1.f, 1.f, 1.f);
	glEnd();

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
				cam->position.y = (objectPoition.y + ((objectScale.y * (Max.y - Min.y)) / 2));
				float h = ((Max.y - Min.y) * objectScale.y) + ((Max.x - Min.x) * objectScale.x) + ((Max.z - Min.z) * objectScale.z);
				float distance = h / math::Atan(75 / 2);
				cam->position.z = (objectPoition.z + distance);
				cam->position.x = (objectPoition.x + distance);
				LookAt({ (((Max.x - Min.x) / 2) + objectPoition.x),(objectPoition.y + ((objectScale.y * (Max.y - Min.y)) / 2)),(((Max.z - Min.z) / 2) + objectPoition.z) });
				cam->frustum.SetPos(vec(cam->position.x, cam->position.y, cam->position.z));
			}
		}
	}

	cam->frustumMatrix = cam->frustum.ViewMatrix(); //Get CameraViewMatrix

	// Recalculate matrix -------------
	CalculateViewMatrix(cam);
	if (GameCam != nullptr) {
		GameCam->frustumMatrix = GameCam->frustum.ViewMatrix(); //Get CameraViewMatrix

	// Recalculate matrix -------------
		CalculateViewMatrix(GameCam);
	}

	//Test if a ray intersects with gameobjects
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		float2 mousePos = { (float)App->input->GetMouseX() ,(float)App->input->GetMouseY() };
		float4 viewport = App->editor->viewport;

		if (mousePos.x > viewport.x && mousePos.x < viewport.x + viewport.z && mousePos.y > viewport.y && mousePos.y < viewport.y + viewport.w)
		{
			int w, h;
			SDL_GetWindowSize(App->window->window, &w, &h); // Si el tamaño de la ventana cambia no te va funcionar usar SCREEN_WIDTH o SCREEN_HEIGHT
			
			float normalized_x = Lerp(-1, 1, mousePos.x / w);
			float normalized_y = Lerp(1, -1, mousePos.y / h);

			App->editor->AddLog("x: %.1f y:%.1f\n", normalized_x, normalized_y);

			cam->frustum.GetCornerPoints(&corners[0]);
			myRay = cam->frustum.UnProjectLineSegment(normalized_x, normalized_y);
			//myRay = cam->frustum.UnProject(normalized_x, normalized_y);

			for (int i = 0; i < App->scene->gameObjects.size(); i++)
			{
				GameObject* go = App->scene->gameObjects[i];
				for (int j = 0; j < go->boundingBoxes.size(); j++)
				{
					if (bool hit = myRay.Intersects(*go->aabb[j]))
					{
						App->editor->AddLog("HIT\n");
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
					float3 distnceVec = hitObjs[i]->components[myComp]->position - cam->frustum.pos;
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
	cam->position = position;
	cam->reference = reference;

	cam->z = normalize(position - reference);
	cam->x = normalize(cross(Vec3(0.0f, 1.0f, 0.0f), cam->z));
	cam->y = cross(cam->z, cam->x);

	if (!rotateAroundReference)
	{
		cam->reference = cam->position;
		cam->position += cam->z * 0.05f;
	}

	CalculateViewMatrix(cam);
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const Vec3& spot)
{
	cam->reference = spot;

	cam->z = normalize(cam->position - cam->reference);
	cam->x = normalize(cross(Vec3(0.0f, 1.0f, 0.0f), cam->z));
	cam->y = cross(cam->z, cam->x);

	CalculateViewMatrix(cam);
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
	cam->position += movement;
	cam->reference += movement;

	CalculateViewMatrix(cam);
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &viewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix(ComponentCamera* cam)
{
	viewMatrix = mat4x4(cam->x.x, cam->y.x, cam->z.x, 0.0f, cam->x.y, cam->y.y, cam->z.y, 0.0f, cam->x.z, cam->y.z, cam->z.z, 0.0f, -dot(cam->x, cam->position), -dot(cam->y, cam->position), -dot(cam->z, cam->position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}