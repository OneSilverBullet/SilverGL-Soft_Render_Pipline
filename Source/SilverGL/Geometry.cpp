#include "Geometry.h"

void AABox::buildAABB(const Mesh & mesh)
{
	VECTOR3D minVals(INT_MAX);
	VECTOR3D maxVals(INT_MIN);

	for (int i = 0; i < mesh.numVertices; ++i)
	{
		minVals.x = min(mesh.vertices[i].x, minVals.x);
		maxVals.x = max(mesh.vertices[i].x, minVals.x);
		minVals.y = min(mesh.vertices[i].y, minVals.y);
		maxVals.y = max(mesh.vertices[i].y, minVals.y);
		minVals.z = min(mesh.vertices[i].z, minVals.z);
		maxVals.z = max(mesh.vertices[i].z, minVals.z);
	}
	minPoint = minVals;
	maxPoint = maxVals;



}

void AABox::update(const MATRIX4X4 & modelMatrix)
{
}
