#include "Transformation.h"

Transformation::Transformation():wx(0),wy(0),wz(0),xrot(0),yrot(0),zrot(0), sc(1.0)
{
	transform_model_matrix.LoadIdentity();
	MATRIX4X4 rotxMatrix = rotateX(xrot);
	MATRIX4X4 rotyMatrix = rotateY(yrot);
	MATRIX4X4 rotzMatrix = rotateZ(zrot);
	MATRIX4X4 transMatrix = translate(wx, wy, wz);
	MATRIX4X4 scaleMatrix = scale(sc);
	transform_model_matrix = transMatrix * rotxMatrix * rotyMatrix *rotzMatrix  * scaleMatrix;
}

Transformation::Transformation(float x, float y, float z)
	: wx(x), wy(y), wz(z), xrot(0), yrot(0), zrot(0), sc(1.0)
{
	transform_model_matrix.LoadIdentity();
	MATRIX4X4 rotxMatrix = rotateX(xrot);
	MATRIX4X4 rotyMatrix = rotateY(yrot);
	MATRIX4X4 rotzMatrix = rotateZ(zrot);
	MATRIX4X4 transMatrix = translate(wx, wy, wz);
	MATRIX4X4 scaleMatrix = scale(sc);
	transform_model_matrix = transMatrix * rotxMatrix * rotyMatrix *rotzMatrix  * scaleMatrix;
}

Transformation::Transformation(float x, float y, float z, float xr, float yr, float zr, float s) 
	: wx(x), wy(y), wz(z), xrot(xr), yrot(yr), zrot(zr), sc(s)
{
	transform_model_matrix.LoadIdentity();
	MATRIX4X4 rotxMatrix = rotateX(xrot);
	MATRIX4X4 rotyMatrix = rotateY(yrot);
	MATRIX4X4 rotzMatrix = rotateZ(zrot);
	MATRIX4X4 transMatrix = translate(wx, wy, wz);
	MATRIX4X4 scaleMatrix = scale(sc);
	transform_model_matrix = transMatrix * rotxMatrix * rotyMatrix *rotzMatrix  * scaleMatrix;
}

void Transformation::Move(float dx, float dy, float dz)
{
	wx += dx;
	wy += dy;
	wz += dz;
	MATRIX4X4 transMatrix = translate(wx, wy, wz);
	transform_model_matrix = transMatrix * transform_model_matrix;
}

void Transformation::Rotate(float xr, float yr, float zr)
{
	xrot = xr;
	yrot = yr;
	zrot = zr;
	MATRIX4X4 rotxMatrix = rotateX(xrot);
	MATRIX4X4 rotyMatrix = rotateY(yrot);
	MATRIX4X4 rotzMatrix = rotateZ(zrot);
	transform_model_matrix = rotxMatrix * rotyMatrix *rotzMatrix  * transform_model_matrix;
}

void Transformation::Scale(float s)
{
	sc = s;
	MATRIX4X4 scaleMatrix = scale(sc);
	transform_model_matrix = scaleMatrix * transform_model_matrix;
}

void Transformation::Reset()
{
	transform_model_matrix.LoadIdentity();
}
