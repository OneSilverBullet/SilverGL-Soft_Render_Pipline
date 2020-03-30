#include "GameObject.h"

GameObject::GameObject()
{
	object_transform = new Transformation();
}

GameObject::GameObject(float x, float y, float z)
{
	object_transform = new Transformation(x,y,z);
}

GameObject::GameObject(float x, float y, float z, float xr, float yr, float zr, float s)
{
	object_transform = new Transformation(x, y, z, xr, yr, zr, s);
}

GameObject::~GameObject()
{
	delete object_transform;
}

MATRIX4X4 GameObject::GetModelMatrix()
{
	return object_transform->transform_model_matrix;
}
