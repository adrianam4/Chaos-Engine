#include "TransformComponent.h"

ComponentTransform::ComponentTransform()
{
	type = ComponentType::TRANSFORM;

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

//void ComponentTransform::CalculateTransMatrix(mat4x4 parentsMatrix, mat4x4 localMatrix)
//{
//	transMatrix = parentsMatrix * localMatrix;
//}
