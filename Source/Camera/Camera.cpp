#include "Camera.h"


Camera::Camera()
{
	std::cout << "Report: Construct a camera" << std::endl;
	//与图像管线联动
	SilverGL::eyex = 1.33;
	SilverGL::eyey = 1.33;
	SilverGL::eyez = -1.4;
	sight = new Sight(SilverGL::eyex, SilverGL::eyey, SilverGL::eyez);
	sight->xrot = -90;
	sight->yrot = 0;

	//camera_viewMatrix.LoadZero();
	//camera_projectMatrix.LoadZero();
	//BuildViewMatrix();
}




void Camera::BuildViewMatrix()
{
	/*
	MATRIX4X4 trans, rotX, rotY;
	rotX = rotateX(sight->yrot);
	rotY = rotateY(sight->xrot);
	trans = translate(sight->sx, sight->sy, sight->sz);
	*/
	VECTOR3D target(0.0, 0.0, 0.0);
	this->camera_viewMatrix = sight->LookAt(target);
}

void Camera::BuildProjectMatrix(int w, int h)
{
	//与管线联动
	SilverGL::clipNear = 0.1;
	float fAspect = (float)w / (float)h;
	MATRIX4X4 a ;
	a= perspective(50.0, fAspect, 0.1, 100.0);
	camera_projectMatrix = a;
}

void Camera::Update()
{
	if (sight != NULL)
	{
		if (move_keys[0])
			sight->moveSight(MNEAR, D_DISTANCE);
		if (move_keys[1])
			sight->moveSight(MFAR, D_DISTANCE);
		if (move_keys[2])
			sight->moveSight(LEFT, D_DISTANCE);
		if (move_keys[3])
			sight->moveSight(RIGHT, D_DISTANCE);
		if (move_keys[4])
			sight->moveSight(UP, D_DISTANCE);
		if (move_keys[5])
			sight->moveSight(DOWN, D_DISTANCE);
		if (turn_keys[0])
			sight->turnY(UP);
		if (turn_keys[1])
			sight->turnY(DOWN);
		if (turn_keys[2])
			sight->turnX(LEFT);
		if (turn_keys[3])
			sight->turnY(RIGHT);
	}
}

bool Camera::checkVisible(AABox * aabb)
{
	return true;
}

VECTOR4D Camera::getPosition()
{
	return VECTOR4D(sight->sx, sight->sy, sight->sz, 1.0);
}
