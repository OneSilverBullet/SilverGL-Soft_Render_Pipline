#ifndef GEOMETRY_H
#define GROMETRY_H

#include "Maths/Maths.h"
#include "Mesh.h"

class AABox
{
public:
	VECTOR3D minPoint;
	VECTOR3D maxPoint;

	void buildAABB(const Mesh& mesh);
	void update(const MATRIX4X4& modelMatrix);

};

#pragma once
#endif // 
