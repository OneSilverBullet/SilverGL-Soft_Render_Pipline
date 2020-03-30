#ifndef UTIL_H
#define UTIL_H

#include "Maths/Maths.h"
#define PI 3.1415926
#define PI2 6.2831852
#define A2R 0.01745329222 //角度转弧度
#define R2A 57.2957804905 //弧度转角度


MATRIX4X4 rotateX(float angle);
MATRIX4X4 rotateY(float angle);
MATRIX4X4 rotateZ(float angle);
MATRIX4X4 scale(float size);
MATRIX4X4 translate(float tx, float ty, float tz);

MATRIX4X4 LookAt(float eyex, float eyey, float eyez,
	float centerX, float centerY, float centerZ,
	float upX, float upY, float upZ);

MATRIX4X4 perspective(float fov, float aspect, float zNear, float zFar);

MATRIX4X4 ortho(float left, float right, float bottom, float top, float n, float f);

VECTOR4D mul(VECTOR4D A, VECTOR4D B)
{
	VECTOR4D res;
	res.x = A.x *B.x;
	res.y = A.y *B.y;
	res.z = A.z *B.z;
	res.w = A.w *B.w;
	return res;

}


float calcZPara(float v1z, float v2z, float z)
{
	return (z - v2z) / (v1z - v2z);
}

inline VECTOR3D calcParaEqu(VECTOR3D vect1, VECTOR3D vect2, float param)
{
	VECTOR3D result;
	result = param * (vect1 - vect2) + vect2;
	return result;
}


inline void interpolate2f(float pa, float pb, float a, float b, float& result)
{
	result = pa * a + pb * b;
}

inline void interpolate3f(float pa, float pb, float pc, float a, float b, float c, float& res)
{
	res = pa * a + pb * b + pc * c;
}

inline float roundf(float number)
{
	return number < 0.0 ? ceilf(number - 0.5) : floorf(number + 0.5);
}

inline float atr(float angle)
{
	return angle * A2R;
}

inline float rta(float rot)
{
	return rot * R2A;
}


#endif // 
