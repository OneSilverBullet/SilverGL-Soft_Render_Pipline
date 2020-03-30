#include "OBJParser.h"

Mesh & OBJ::buildMeshFromFile(Mesh & mesh, std::string path)
{
	// 加载模型具体的路径，并加载mesh
	std::ifstream file;
	file.open(path.c_str());
	loadFileData(mesh, file);
	file.close();
	return mesh;
}

bool OBJ::fileExists(std::string & path)
{
	std::ifstream file(path);
	return file.good();
}

void OBJ::loadFileData(Mesh & mesh, std::ifstream & file)
{
	std::string line, key, x, y, z;
	float tempU, tempV, intpart;
	VECTOR3I indices[3];
	char delimeter = '/';
	while (!file.eof()) {
		std::getline(file, line);
		std::istringstream iss(line);
		iss >> key;
		if (key == "v")
		{
			iss >> x >> y >> z;
			VECTOR3D vertex(std::stof(x), std::stof(y), std::stof(z));
			mesh.vertices.push_back(vertex);
		}
		else if (key == "vn")
		{
			iss >> x >> y >> z;
			VECTOR3D normal(std::stof(x), std::stof(y), std::stof(z));
			mesh.normals.push_back(normal);
		}
		else if (key == "vt")
		{
			iss >> x >> y;
			VECTOR3D tex(std::stof(x), std::stof(y), 0);
			mesh.texels.push_back(tex);
		}
		else if (key == "f")
		{
			iss >> x >> y >> z;
			std::vector<std::string> splitX = splitStr(x, delimeter);
			std::vector<std::string> splitY = splitStr(y, delimeter);
			std::vector<std::string> splitZ = splitStr(z, delimeter);
			for (int i = 0; i < splitX.size(); ++i)
			{
				indices[i] = VECTOR3I(std::stoi(splitX[i]) - 1, std::stoi(splitY[i]) - 1, std::stoi(splitZ[i]) - 1);
			}
			mesh.fVertexIndices.push_back(indices[0]);
			mesh.fTextureIndices.push_back(indices[1]);
			mesh.fNormalsIndices.push_back(indices[2]);
		}
	}
	mesh.numVertices = mesh.vertices.size();
	mesh.numFaces = mesh.fVertexIndices.size();
	file.clear();
	file.seekg(0, file.beg);
}

std::vector<std::string> OBJ::splitStr(std::string & str, char delim)
{
	std::stringstream ss(str);
	std::string token;
	std::vector<std::string> splitString;
	while (std::getline(ss, token, delim))
	{
		if (token == "")
		{
			splitString.push_back("0");
		}
		else
		{
			splitString.push_back(token);
		}
	}
	return splitString;
}
