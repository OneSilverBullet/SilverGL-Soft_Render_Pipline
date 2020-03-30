#ifndef CUBE_H
#define CUBE_H

#include "GraphicLibs.h"
#include "GameObject.h"

class Cube : public GameObject
{
private:
	int facenum;
	void initVerts();
public:
	Vertex* verts;
	Cube();
	Cube(float x, float y, float z);
	Cube(float x, float y, float z, float xr, float yr, float zr, float s);
	~Cube();
	void render(FrameBuffer* fb, DepthBuffer* db, VertexShader vs, FragmentShader fs, int cullFlag);
};



#endif