#include "ResourceManager.h"

std::map<std::string, ShaderProgram*> ResourceManager::Shaders;
std::map<std::string, Texture*> ResourceManager::Textures;

ShaderProgram* ResourceManager::LoadShaderProgram(VertexShader vs, FragmentShader fs, std::string n)
{
	if (Shaders.find(n) != Shaders.end())
	{
		std::cout << "Error : shader name conflict!" << std::endl;
		return nullptr;
	}
	ShaderProgram* curprogram = new ShaderProgram(n, vs, fs);
	Shaders[n] = curprogram;
	return curprogram;
}

ShaderProgram* ResourceManager::GetShaderProgram(std::string name)
{
	return Shaders[name];
}

Texture * ResourceManager::LoadTexture(std::string dir, std::string name)
{
	if (Textures.find(name) != Textures.end())
	{
		std::cout << "Error: Texture load conflict!" << std::endl;
		return nullptr;
	}
	Texture* curtexture = new Texture(dir.c_str(), "other");
	Textures[name] = curtexture;
	return curtexture;
}

Texture * ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Release()
{
	for (auto x : Shaders)
	{
		delete x.second;
	}
	for (auto x : Textures)
	{
		delete x.second;
	}
}
