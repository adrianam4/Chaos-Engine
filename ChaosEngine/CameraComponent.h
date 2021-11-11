#include "Globals.h"
#include "Module.h"
#include "glmath.h"
#include <vector>

class Component;

class ComponentCamera : public Component
{
public:
	ComponentCamera(float3 pos, double hFov, double nPlane, double fPlane);
	~ComponentCamera();

	void Enable() override;
	void Update() override;
	void Disable() override;
	void OnEditor(int i) override;
	void Draw();

private:
	Frustum frustum;
	unsigned int VBO;
	unsigned int EBO;
	std::vector<float3> vertices;
	std::vector<float> indices;

	void RecalculateFront(float degrees);
	void RecalculateUp(float degrees);
	void RecalculateCamera();
	void CalculatePoints();
};