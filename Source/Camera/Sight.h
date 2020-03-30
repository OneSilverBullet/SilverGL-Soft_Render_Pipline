#ifndef SIGHT_H
#define SIGHT_H

#include "Util.h"
#include "Constants.h"

class Sight
{
public:
	float sx, sy, sz, sh;
	float xrot, yrot, zrot;

	Sight(float x, float y, float z);
	~Sight();
	void turnY(int ud);
	void turnX(int lr);
	void moveSight(int dist, float speed);
	MATRIX4X4 LookAt(VECTOR3D& target, VECTOR3D temp = VECTOR3D(0,1,0));

};


#endif