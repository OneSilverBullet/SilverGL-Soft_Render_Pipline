#include "Sight.h"

Sight::Sight(float x, float y, float z)
{
	sx = x;
	sy = y;
	sz = z;
	sh = 2;
	xrot = 0;
	yrot = 0;
	zrot = 0;
}

Sight::~Sight()
{
}

void Sight::turnY(int ud)
{
	switch (ud)
	{
	case UP:
		yrot -= D_ROTATION;
		break;
	case DOWN:
		yrot += D_ROTATION;
		break;
	}
	if (yrot > 360)
		yrot -= 360;
	else if (yrot < 0)
		yrot += 360;
}

void Sight::turnX(int lr)
{
	switch (lr)
	{
	case LEFT:
		xrot -= D_ROTATION;
		break;
	case RIGHT:
		xrot += D_ROTATION;
		break;
	}
	if (xrot > 360)
	{
		xrot -= 360.0;
	}
	else if (xrot < 0)
	{
		xrot += 360.0;
	}
}

void Sight::moveSight(int dist, float speed)
{
	float xz = atr(xrot); //将角度转为x的弧度
	float yz = atr(yrot);//将角度转为y的弧度
	float dx = speed * cos(yz) * sin(xz);
	float dy = speed * sin(yz);
	float dz = speed * cos(yz) * cos(xz);

	switch (dist)
	{
	case DOWN:
		sy += speed;
		sh -= speed;
		break;
	case UP:
		sy -= speed;
		sh += speed;
		break;
	case RIGHT:
		sx -= speed * cos(xz);
		sz -= speed * sin(xz);
		break;
	case LEFT:
		sx += speed * cos(xz);
		sz += speed * sin(xz);
		break;
	case MFAR:
		sx += dx;
		sy -= dy;
		sz -= dz;
		break;
	case MNEAR:
		sx -= dx;
		sy += dy;
		sz += dz;
		break;
	}



}

MATRIX4X4 Sight::LookAt(VECTOR3D& target, VECTOR3D temp)
{
	VECTOR3D position(sx, sy, sz);

	VECTOR3D forward = (position - target);
	forward.Normalize();
	VECTOR3D side = (temp.CrossProduct(forward));
	side.Normalize();
	VECTOR3D up = forward.CrossProduct(side);

	//We will now build the inverse transform from the world position to the camera
	//The idea is that we don't care where the camera is, we only care about what
	//transformation would put the origin at the camera world space position
	//With the z axis behind the camera.
	MATRIX4X4 worldToCam;

	worldToCam.entries[0] = side.x;
	worldToCam.entries[1] = side.y;
	worldToCam.entries[2] = side.z;
	worldToCam.entries[3] = -side.DotProduct(position);

	worldToCam.entries[4] = up.x;
	worldToCam.entries[5] = up.y;
	worldToCam.entries[6] = up.z;
	worldToCam.entries[7] = -up.DotProduct(position);

	worldToCam.entries[8] = forward.x;
	worldToCam.entries[9] = forward.y;
	worldToCam.entries[10] = forward.z;
	worldToCam.entries[11] = -forward.DotProduct(position);

	worldToCam.entries[15] = 1;


	return worldToCam;

}
