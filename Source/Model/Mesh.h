#ifndef MESH_H
#define MESH_H

#include "Maths/Maths.h"
#include "Header.h"

class Mesh
{
public:
	//���㼶����Ϣ
	int numVertices = 0;
	std::vector<VECTOR3D> vertices; //������Ϣ
	std::vector<VECTOR3D> normals;
	std::vector<VECTOR3D> texels;
	std::vector<VECTOR3D> tangents;
	std::vector<VECTOR3D> biTangents;

	//��Ƭ������Ϣ
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