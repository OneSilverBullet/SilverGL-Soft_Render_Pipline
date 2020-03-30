#ifndef OBJPARSER_H
#define  OBJPARSER_H

#include <fstream>
#include <sstream>
#include "Mesh.h"

namespace OBJ
{
	Mesh& buildMeshFromFile(Mesh& mesh, std::string path);
	bool fileExists(std::string& path);
	void loadFileData(Mesh& mesh, std::ifstream& file);
	std::vector<std::string> splitStr(std::string & str, char delim);


}



#endif