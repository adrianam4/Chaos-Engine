#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

class Component;
class ModuleViewportFrameBuffer;
class ComponentCamera : public Component
{
public:
	ComponentCamera(float3 pos, double hFov, double nPlane, double fPlane, bool isGameObj);
	~ComponentCamera()override;

	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;
	void pre();
	void post();
	void Draw() override;

public:
	Vec3 position, reference, x, y, z;
	Frustum frustum;
	unsigned int VBO;
	unsigned int EBO;
	std::vector<float3> vertices;
	std::vector<uint> indices;
	bool isTheMainCamera;
	float frontRotation;
	float upRotation;
	void changeViewMatrix();
	mat4x4 viewMatrix;
	mat4x4 getViewmatrix();
	void RecalculateRotation(float xDegrees, float yDegrees);
	void RecalculateCamera();
	void CalculatePoints();
	float4x4 frustumMatrix;
	float4x4 projMatrix;
	bool start = true;
	uint frameBuffer = 0;
	uint renderBufferoutput = 0;
	uint texture = 0;
	bool show_viewport_window = true;
	float2 size;
};