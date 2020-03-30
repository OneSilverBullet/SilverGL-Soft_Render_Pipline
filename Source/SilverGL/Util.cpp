#include "Util.h"


MATRIX4X4 rotateX(float angle) {
	float radian = angle * A2R;
	float sinA = sinf(radian);
	float cosA = cosf(radian);
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[5] = cosA;
	mat.entries[6] = sinA;
	mat.entries[9] = -sinA;
	mat.entries[10] = cosA;
	return mat;
}

MATRIX4X4 rotateY(float angle) {
	float radian = angle * A2R;
	float sinA = sinf(radian);
	float cosA = cosf(radian);
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0] = cosA;
	mat.entries[2] = -sinA;
	mat.entries[8] = sinA;
	mat.entries[10] = cosA;
	return mat;
}

MATRIX4X4 rotateZ(float angle) {
	float radian = angle * A2R;
	float sinA = sinf(radian);
	float cosA = cosf(radian);
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0] = cosA;
	mat.entries[1] = sinA;
	mat.entries[4] = -sinA;
	mat.entries[5] = cosA;
	return mat;
}

MATRIX4X4 scale(float size) {
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0] = size;
	mat.entries[5] = size;
	mat.entries[10] = size;
	return mat;
}

MATRIX4X4 translate(float tx, float ty, float tz) {
	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[12] = tx;
	mat.entries[13] = ty;
	mat.entries[14] = tz;
	return mat;
}



MATRIX4X4 LookAt(float eyex, float eyey, float eyez, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	VECTOR3D eye;
	eye.x = eyex;
	eye.y = eyey;
	eye.z = eyez;
	VECTOR3D center;
	center.x = centerX;
	center.y = centerY;
	center.z = centerZ;
	VECTOR3D up;
	up.x = upX;
	up.y = upY;
	up.z = upZ;

	VECTOR3D front = center - eye;
	front.Normalize();
	VECTOR3D right = front.CrossProduct(up);
	right.Normalize();
	up = right.CrossProduct(front);
	up.Normalize();

	MATRIX4X4 mat;
	mat.LoadIdentity();
	mat.entries[0] = right.x;
	mat.entries[4] = right.y;
	mat.entries[8] = right.z;
	mat.entries[1] = up.x;
	mat.entries[5] = up.y;
	mat.entries[9] = up.z;
	mat.entries[2] = -front.x;
	mat.entries[6] = -front.y;
	mat.entries[10] = -front.z;
	mat.entries[12] = -(right.DotProduct(eye));
	mat.entries[13] = -(up.DotProduct(eye));
	mat.entries[14] = (front.DotProduct(eye));
	return mat;
}

MATRIX4X4 perspective(float fovy, float aspect, float zNear, float zFar)
{
	float rFovy = fovy * PI / 180;
	float tanHalfFovy = tanf(rFovy / 2);

	MATRIX4X4 mat;
	mat.LoadIdentity();
	float tanHalfFOVInverse = 1 / tan((fovy / 2) * (PI / 180));
	mat.entries[0] = tanHalfFOVInverse;
	mat.entries[5] = aspect * tanHalfFOVInverse;
	mat.entries[10] = zNear / (zFar - zNear);
	mat.entries[11] = zFar * zNear / (zFar - zNear);
	mat.entries[14] = -1;
	mat.entries[15] = 0;

	/*
	mat.entries[0] = 1 / (aspect * tanHalfFovy);
	mat.entries[5] = 1 / tanHalfFovy;
	mat.entries[10] = -(zFar + zNear) / (zFar - zNear);
	mat.entries[11] = -1;
	mat.entries[14] = -2 * zFar*zNear / (zFar - zNear);
	mat.entries[15] = 0;
	*/
	return mat;
}

MATRIX4X4 ortho(float left, float right, float bottom, float top, float n, float f)
{
	return MATRIX4X4();
}
