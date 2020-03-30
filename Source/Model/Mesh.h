#ifndef MESH_H
#define MESH_H

#include "Maths/Maths.h"
#include "Header.h"

class Mesh
{
public:
	//顶点级别信息
	int numVertices = 0;
	std::vector<VECTOR3D> vertices; //坐标信息
	std::vector<VECTOR3D> normals;
	std::vector<VECTOR3D> texels;
	std::vector<VECTOR3D> tangents;
	std::vector<VECTOR3D> biTangents;

	//面片级别信息
	int numFaces = 0;
	std::vector<VECTOR3D> fNormals;
	std::vector<VECTOR3I> fVertexIndices;
	std::vector<VECTOR3I> fTextureIndices;
	std::vector<VECTOR3I> fNormalsIndices;
	
	void describeMesh();
	void buildFaceNormals();
	void buildTangentSpace();


};




#endif