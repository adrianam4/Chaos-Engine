#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleScene.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	CalculateViewMatrix();

	x = Vec3(1.0f, 0.0f, 0.0f);
	y = Vec3(0.0f, 1.0f, 0.0f);
	z = Vec3(0.0f, 0.0f, 1.0f);

	position = Vec3(0.0f, 0.0f, 5.0f);
	reference = Vec3(0.0f, 0.0f, 0.0f);

	vec pos = vec(0.0f, 0.0f, 5.0f);
	vec front = vec(1.0f, 0.0f, 0.0f);
	vec up = vec(0.0f, 1.0f, 0.0f);
	
	float horizontalFov, verticalFov;
	horizontalFov = verticalFov = 75;
	camFrustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	camFrustum.SetViewPlaneDistances(5, 300);
	camFrustum.SetFrame(pos, front, up);
	camFrustum.SetPos(pos);
	camFrustum.SetFront(front);
	camFrustum.SetUp(up);
	camFrustum.SetPerspective(horizontalFov, verticalFov);
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

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= z * speed * 2;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += z * speed * 2;

	if (App->input->GetMouseZ() > 0) newPos -= z * speed * 6;
	if (App->input->GetMouseZ() < 0) newPos += z * speed * 6;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= x * speed * 2;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += x * speed * 2;
	position += newPos;
	reference += newPos;

	// Mouse motion ----------------
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT || (App->input->GetMouseButton(SDL_BUTTON_LEFT) && App->input->GetKey(SDL_SCANCODE_LALT)))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float sensitivity = 0.25f;

		position -= reference;

		if (dx != 0)
		{
			float deltaX = (float)dx * sensitivity;

			x = rotate(x, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			y = rotate(y, deltaX, Vec3(0.0f, 1.0f, 0.0f));
			z = rotate(z, deltaX, Vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float deltaY = (float)dy * sensitivity;

			y = rotate(y, deltaY, x);
			z = rotate(z, deltaY, x);

			if (y.y < 0.0f)
			{
				z = Vec3(0.0f, z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				y = cross(z, x);
			}
		}

		position = reference + z * length(position);
	}

	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		if (App->editor->objectSelected == NULL)
		{
			Look(Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f), true);
		}
		else
		{
			/*float3 objectPoition=App->editor->objectSelected->trans;*/
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



			position.y = (objectPoition.y + ((objectScale.y * (Max.y - Min.y)) / 2));

			float h = ((Max.y - Min.y) * objectScale.y) + ((Max.x - Min.x) * objectScale.x) + ((Max.z - Min.z) * objectScale.z);
			float distance = h / math::Atan(75 / 2);



			position.z = (objectPoition.z + distance);
			position.x = (objectPoition.x + distance);
			LookAt({ (((Max.x - Min.x) / 2) + objectPoition.x),(objectPoition.y + ((objectScale.y * (Max.y - Min.y)) / 2)),(((Max.z - Min.z) / 2) + objectPoition.z) });
		}
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const Vec3 &position, const Vec3 &reference, bool rotateAroundReference)
{
	this->position = position;
	this->reference = reference;

	z = normalize(position - reference);
	x = normalize(cross(Vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	if(!rotateAroundReference)
	{
		this->reference = this->position;
		this->position += z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const Vec3 &spot)
{
	reference = spot;

	z = normalize(position - reference);
	x = normalize(cross(Vec3(0.0f, 1.0f, 0.0f), z));
	y = cross(z, x);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const Vec3 &movement)
{
	position += movement;
	reference += movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &viewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	viewMatrix = mat4x4(x.x, y.x, z.x, 0.0f, x.y, y.y, z.y, 0.0f, x.z, y.z, z.z, 0.0f, -dot(x, position), -dot(y, position), -dot(z, position), 1.0f);
	viewMatrixInverse = inverse(viewMatrix);
}