#include "Mesh.h"

void Mesh::describeMesh()
{
	std::cout << "Describe Mesh: " << std::endl;
	for (int i = 0; i < numVertices; ++i)
	{
		std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
	}
	std::cout << "Mesh vertex number : " << numVertices << std::endl;
}

void Mesh::buildFaceNormals()
{
	for (int i = 0; i < numFaces; ++i)
	{
		VECTOR3I indices = fVertexIndices[i];
		VECTOR3D N1 = vertices[indices.y] - vertices[indices.x];
		VECTOR3D N2 = vertices[indices.z] - vertices[indices.x];
		VECTOR3D N3 = N1.CrossProduct(N2);
		N3.Normalize();
		fNormals.push_back(N3);
	}
}

void Mesh::buildTangentSpace()
{
	std::vector<std::vector<VECTOR3D>> tempTangents(numVertices);
	std::vector<std::vector<VECTOR3D>> tempBiTangents(numVertices);
	VECTOR3D tangent, biTangent;

	//Extract the tangent and bitangentn of each surface triangle
   //Assign the value to a temporary vector of vectors of vector3's (yikes)
   //¡°When I wrote this, only God and I understood what I was doing. Now, God only knows.¡±
	for (int i = 0; i < numFaces; ++i) {
		VECTOR3I vIndices = fVertexIndices[i];
		VECTOR3I tIndices = fTextureIndices[i];

		VECTOR3D v0 = vertices[vIndices.x];
		VECTOR3D v1 = vertices[vIndices.y];
		VECTOR3D v2 = vertices[vIndices.z];

		VECTOR3D edge1 = v1 - v0;
		VECTOR3D edge2 = v2 - v0;

		VECTOR3D deltaUV1 = texels[tIndices.y] - texels[tIndices.x];
		VECTOR3D deltaUV2 = texels[tIndices.z] - texels[tIndices.x];

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent.Normalize();

		biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		biTangent.Normalize();

		tempTangents[vIndices.x].push_back(tangent);
		tempTangents[vIndices.y].push_back(tangent);
		tempTangents[vIndices.z].push_back(tangent);

		tempBiTangents[vIndices.x].push_back(biTangent);
		tempBiTangents[vIndices.y].push_back(biTangent);
		tempBiTangents[vIndices.z].push_back(biTangent);

	}


	//Take average of tangents and bitangents at that vertex and populate the array in the correct order
	for (int j = 0; j < numVertices; ++j) {
		int count1 = 0, count2 = 0;

		VECTOR3D temp1{ 0.0 };
		while (!tempTangents[j].empty()) {
			++count1;
			temp1 += tempTangents[j].back();
			if (temp1.x == 0 && temp1.y == 0 && temp1.z == 0) {
				temp1 = tempTangents[j].back();
			}
			tempTangents[j].pop_back();
		}

		temp1 = temp1 * (1.0f / count1);
		tangents.push_back(temp1);

		VECTOR3D temp2{ 0.0 };
		while (!tempBiTangents[j].empty()) {
			++count2;
			temp2 += tempBiTangents[j].back();
			if (temp2.x == 0 && temp2.y == 0 && temp2.z == 0) {
				temp2 = tempBiTangents[j].back();
			}
			tempBiTangents[j].pop_back();
		}
		temp2 = temp2 * (1.0f / count2);
		biTangents.push_back(temp2);
	}

	//Renormalize and correct handedness of tangent/bitangent vectors
	for (int k = 0; k < numFaces; ++k) {
		VECTOR3I vIndices = fVertexIndices[k];
		VECTOR3I nIndices = fNormalsIndices[k];

		//Per vertex
		VECTOR3D tangentV0 = tangents[vIndices.x];
		VECTOR3D tangentV1 = tangents[vIndices.y];
		VECTOR3D tangentV2 = tangents[vIndices.z];

		VECTOR3D normalV0 = normals[nIndices.x];
		VECTOR3D normalV1 = normals[nIndices.y];
		VECTOR3D normalV2 = normals[nIndices.z];

		VECTOR3D biTangentV0 = biTangents[vIndices.x];
		VECTOR3D biTangentV1 = biTangents[vIndices.y];
		VECTOR3D biTangentV2 = biTangents[vIndices.z];

		//Renormalizing
		tangentV0 = (tangentV0 - (normalV0*tangentV0.DotProduct(normalV0)));
		tangentV0.Normalize();
		tangentV1 = (tangentV1 - (normalV1*tangentV1.DotProduct(normalV1)));
		tangentV1.Normalize(); 
		tangentV2 = (tangentV2 - (normalV2*tangentV2.DotProduct(normalV2)));
		tangentV2.Normalize(); 

		//Correcting handedness
		if (biTangentV0.DotProduct(normalV0.CrossProduct(tangentV0)) < 0.0f) {
			tangentV0 = tangentV0 * -1.0f;
		}

		if (biTangentV1.DotProduct(normalV1.CrossProduct(tangentV1)) < 0.0f) {
			tangentV1 = tangentV1 * -1.0f;
		}

		if (biTangentV2.DotProduct(normalV2.CrossProduct(tangentV2)) < 0.0f) {
			tangentV2 = tangentV2 * -1.0f;
		}

		tangents[vIndices.x] = tangentV0;
		tangents[vIndices.y] = tangentV1;
		tangents[vIndices.z] = tangentV2;

	}
}
