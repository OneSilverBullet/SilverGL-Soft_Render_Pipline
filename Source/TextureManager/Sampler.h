#ifndef SAMPLER_H
#define SAMPLER_H

#include "Maths/Maths.h"
#include "Header.h"

class Sampler
{
public:
	int width, height, channels;
	int tileW = 32, tileH = 32, widthInTiles;
	float* imgData;

	Sampler(int sw, int sh, int sc);
	~Sampler();
	VECTOR4D texture2D(float s, float t); //读取图像颜色
	void tileData();
private:
	VECTOR4D texture2D_TileData(float u, float v);
	VECTOR4D texture2D_TileData_Single(float u, float v);
	VECTOR4D texture2D_Nearest(float s, float t);//读取三通道图
	VECTOR4D texture2D_Linear(float s, float t);//读取三通道图
	VECTOR4D texture2D_Linear_Depth(float s, float t);//读取单通道图
};

void writeFrameBuffer2Sampler(FrameBuffer* fb, Sampler* sampler);



#endif