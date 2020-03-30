#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Transformation.h"

class GameObject
{
public:
	Transformation* object_transform;
	GameObject();
	GameObject(float x, float y, float z);
	GameObject(float x, float y, float z, float xr, float yr, float zr, float s);
	~GameObject();

	MATRIX4X4 GetModelMatrix();
};



#endif // 
