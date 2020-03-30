#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "Maths/Maths.h"
#include "Header.h" 

class Transformation {
public:
	float wx, wy, wz;
	float xrot, yrot, zrot;
	float sc;

	MATRIX4X4 transform_model_matrix;

	Transformation();
	Transformation(float x, float y, float z);
	Transformation(float x, float y, float z, float xr, float yr, float zr, float s);
	void Move(float dx, float dy, float dz);
	void Rotate(float xr, float yr, float zr);
	void Scale(float s);
	void Reset();
};



#endif
