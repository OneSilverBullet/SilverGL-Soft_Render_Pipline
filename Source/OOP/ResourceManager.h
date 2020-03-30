#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Header.h"
#include "Texture.h"
#include "ShaderProgram.h"

class ResourceManager
{
public:
	static std::map<std::string, ShaderProgram*> Shaders;
	static std::map<std::string, Texture*> Textures;

	static ShaderProgram* LoadShaderProgram(VertexShader vs, FragmentShader fs, std::string n);
	static ShaderProgram* GetShaderProgram(std::string name);
	static Texture* LoadTexture(std::string  dir, std::string name);
	static Texture* GetTexture(std::string name);
	static void Release();

private:
	ResourceManager() {}
};




#endif // 
