#ifndef TEXTURE_H
#define TEXTURE_H

#include "Sampler.h"
#include "BmpLoader.h"



class Texture
{
public:
	Sampler* sampler;
	Texture(std::string src, std::string type);
	~Texture();
	VECTOR4D texture2D(float s, float t);
	int width, height, channels;//当前texture的读取
	int tileW = 32, tileH = 32; //用于分块扩展
	

};




#endif