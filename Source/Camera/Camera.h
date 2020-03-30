#ifndef CAMERA_H
#define CAMERA_H

#include "Sight.h"
#include "GraphicLibs.h"
#include "Keyboard.h"
#include "Geometry.h"

enum CAMERA_TYPE {
	PERSPECTIVE, ORTHO
};

class Camera
{
public:
	Sight* sight;
	MATRIX4X4 camera_projectMatrix, camera_viewMatrix;
	CAMERA_TYPE camera_type;

	Camera();
	void BuildViewMatrix();
	
	void BuildProjectMatrix(int w, int h);
	void Update();
	bool checkVisible(AABox* aabb);
	VECTOR4D getPosition();
};




#endif