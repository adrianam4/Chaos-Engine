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
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
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
			float3 objectPosition = App->editor->objectSelected->getTransform()->position;
			float3 Max = App->editor->objectSelected->aabb[0]->maxPoint;
			float3 Min = App->editor->objectSelected->aabb[0]->minPoint;

			position.y = ((Max.y - Min.y) / 2) + objectPosition.y;

			float h = (Max.y - Min.y) + (Max.x - Min.x) + (Max.z - Min.z);
			float distance = h / math::Atan(75 / 2);
			distance += 2;
			position.z = objectPosition.z + distance / 2;
			position.x = objectPosition.x + distance / 2;
			LookAt({ (((Max.x - Min.x) / 2) + Min.x) + objectPosition.x,(((Max.y - Min.y) / 2) + Min.y) + objectPosition.y,(((Max.z - Min.z) / 2) + Min.z) + objectPosition.z });
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) && App->input->GetKey(SDL_SCANCODE_LALT))
	{
		if (App->editor->objectSelected == NULL)
		{
			LookAt(Vec3(0.0f, 0.0f, 0.0f));
			position = (Vec3(3.0f, 3.0f, 3.0f));
		}
		else
		{
			float3 objectPoition = App->editor->objectSelected->getTransform()->position;
			float3 Max = App->editor->objectSelected->aabb[0]->maxPoint;
			float3 Min = App->editor->objectSelected->aabb[0]->minPoint;

			position.y = ((Max.y - Min.y) / 2) + objectPoition.y;

			float h = (Max.y - Min.y) + (Max.x - Min.x) + (Max.z - Min.z);
			float distance = h / math::Atan(75 / 2);
			distance += 2;
			position.z = objectPoition.z + distance / 2;
			position.x = objectPoition.x + distance / 2;
			LookAt({ ((Max.x - Min.x) / 2) + Min.x,((Max.y - Min.y) / 2) + Min.y,((Max.z - Min.z) / 2) + Min.z });
		}

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

		}

		position = reference + z * length(position);
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