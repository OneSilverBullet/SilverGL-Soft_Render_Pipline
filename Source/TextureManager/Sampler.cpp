#include "Sampler.h"

Sampler::Sampler(int sw, int sh, int sc)
{
	width = sw;
	height = sh;
	channels = sc;
	imgData = new  float[width * height * channels];
	memset(imgData, 0, width * height * channels * sizeof(float));
	widthInTiles = (width + tileW - 1) / tileW;
}

Sampler::~Sampler()
{
	delete[] imgData;
	printf("release sampler\n");
}

VECTOR4D Sampler::texture2D(float s, float t)
{
	if (channels == 1)
	{
		return texture2D_TileData_Single(s, t);
	}
	else if (channels == 3)
	{

		return texture2D_TileData(s, t);
		//return texture2D_Linear(s, t);
	}
	else
	{
		std::cout << "Error : the channel is not 1 or 3!" << std::endl;
	}
	return VECTOR4D();
}


//Organizes the texture images in tiles to improve cache coherency
//and reduce the amount of cache misses that getting pixel values would cause
void Sampler::tileData() {
	float *tiledPixelData = new float[width*height*channels];

	//将当前的图像分成32x32的小块，计算得到这些小块长宽各有几块
	int tileNumW = width / tileW;
	int tileNumH = height / tileH;
	int linearIndex = 0;
	int tiledIndex = 0;

	for (int tileRow = 0; tileRow < tileNumW; ++tileRow) {//遍历宽有几块

		for (int tileCol = 0; tileCol < tileNumH; ++tileCol) {//遍历高有几块

			for (int tilePixelHeight = 0; tilePixelHeight < tileH; ++tilePixelHeight) {//遍历当前块的高

				//1.First multiplication accounts for a change in pixel height within a tile
				//2.Second accounts for a change of tile along arrow(tile movement column wise)
				//3.Third accounts for the movement of one whole tile row downwards
				//计算得到当前块的线性index。
				linearIndex = (tilePixelHeight*width + tileCol * tileW + tileRow * width*tileH)*channels;
				//遍历当前块的的当前行的当前像素的所有通道，并且将数据加入到tiledPixelData当中
				for (int tilePixelWidth = 0; tilePixelWidth < tileW; ++tilePixelWidth) {
					//Pixel wise movement is equal to channelwise movement in the array
					for (int pC = 0; pC < channels; ++pC) {

						tiledPixelData[tiledIndex] = imgData[linearIndex];
						++linearIndex;
						++tiledIndex;

					}
				}
			}
		}
	}
	delete[] imgData;
	imgData = tiledPixelData;
}



//Linear插值相近的4个坐标点，来达到平滑的采样效果
VECTOR4D Sampler::texture2D_Linear(float s, float t)
{
	//OpenGL的坐标系中y轴是从下往上递增，而存储的坐标系y轴是从上往下递增
	VECTOR4D  color(1, 1, 1, 1), nextu_color(1, 1, 1, 1), nextv_color(1, 1, 1, 1), nextuv_color(1, 1, 1, 1);
	float u = (float)(width - 1) * s;
	float v = (float)(height - 1)*(1.0 - t);
	int iu = (int)u;
	int iv = (int)v;
	int nextu = (iu + 1) <= (width - 2) ? (iu+1) : iu;
	int nextv = (iv + 1) <= (height - 2) ? (iv + 1) : iv;
	/*
	float uW = u - iu;
	float vW  = v - iv;
	float uNextW = 1.0 - uW;
	float vNextW = 1.0 - vW;
	*/
	float uNextW = u - iu;
	float vNextW = v - iv;
	float uW = 1.0 - uNextW;
	float vW = 1.0 - vNextW;

	int o_index = (iu * width + iv) *3;
	color.x = (float)imgData[o_index] * INV_SCALE;
	color.y = (float)imgData[o_index + 1] * INV_SCALE;
	color.z = (float)imgData[o_index + 2] * INV_SCALE;

	int nextv_index = (iu * width + nextv) * 3;
	nextv_color.x = (float)imgData[nextv_index] * INV_SCALE;
	nextv_color.y = (float)imgData[nextv_index + 1] * INV_SCALE;
	nextv_color.z = (float)imgData[nextv_index + 2] * INV_SCALE;

	int nextu_index = (nextu*width + iv) * 3;
	nextu_color.x = (float)imgData[nextu_index] * INV_SCALE;
	nextu_color.y = (float)imgData[nextu_index + 1] * INV_SCALE;
	nextu_color.z = (float)imgData[nextu_index + 2] * INV_SCALE;

	int nextuv_index = (nextu*width + nextv) * 3;
	nextuv_color.x = (float)imgData[nextuv_index] * INV_SCALE;
	nextuv_color.y = (float)imgData[nextuv_index + 1] * INV_SCALE;
	nextuv_color.z = (float)imgData[nextuv_index + 2] * INV_SCALE;

	//插值
	color.x = color.x*uW*vW + nextu_color.x*uNextW*vW + nextv_color.x * uW * vNextW + nextuv_color.x * uNextW *vNextW;
	color.y = color.y*uW*vW + nextu_color.y*uNextW*vW + nextv_color.y * uW * vNextW + nextuv_color.y * uNextW *vNextW;
	color.z = color.z*uW*vW + nextu_color.z*uNextW*vW + nextv_color.z * uW * vNextW + nextuv_color.z * uNextW *vNextW;
	//std::cout << "color sampler : " << color.x << " "<<color.y << " " << color.z << " " << std::endl;

	return color;
}

VECTOR4D Sampler::texture2D_Linear_Depth(float s, float t)
{
	//OpenGL的坐标系中y轴是从下往上递增，而存储的坐标系y轴是从上往下递增
	VECTOR4D  color(1, 0, 0, 1), nextu_color(1, 0, 0, 1), nextv_color(1, 0, 0, 1), nextuv_color(1, 0, 0, 1);
	float u = (float)(width - 1) * s;
	float v = (float)(height - 1)*(1.0 - t);
	int iu = (int)u;
	int iv = (int)v;
	int nextu = (iu + 1) <= (width - 2) ? (iu + 1) : iu;
	int nextv = (iv + 1) <= (height - 2) ? (iv + 1) : iv;
	/*
	float uW = u - iu;
	float vW  = v - iv;
	float uNextW = 1.0 - uW;
	float vNextW = 1.0 - vW;
	*/
	float uNextW = u - iu;
	float vNextW = v - iv;
	float uW = 1.0 - uNextW;
	float vW = 1.0 - vNextW;

	int o_index = (iu * width + iv) ;
	color.x = (float)imgData[o_index] * INV_SCALE;

	int nextv_index = (iu * width + nextv) ;
	nextv_color.x = (float)imgData[nextv_index] * INV_SCALE;


	int nextu_index = (nextu*width + iv) ;
	nextu_color.x = (float)imgData[nextu_index] * INV_SCALE;

	int nextuv_index = (nextu*width + nextv) ;
	nextuv_color.x = (float)imgData[nextuv_index] * INV_SCALE;

	//插值
	color.x = color.x*uW*vW + nextu_color.x*uNextW*vW + nextv_color.x * uW * vNextW + nextuv_color.x * uNextW *vNextW;
	color.y = 0;
	color.z = 0;
	//std::cout << "color sampler : " << color.x << " "<<color.y << " " << color.z << " " << std::endl;

	return color;
}

VECTOR4D Sampler::texture2D_TileData(float u, float v)
{
	int uInt = u * (width - 1);
	int vInt = v * (height - 1);

	int tileX = uInt / tileW;
	int tileY = vInt / tileH;

	int inTileX = uInt % tileW;
	int inTileY = vInt % tileH;

	int index = ((tileY * widthInTiles + tileX) * (tileW * tileH)
		+ inTileY * tileW + inTileX)*channels;

	return VECTOR4D(imgData[index], imgData[index + 1], imgData[index + 2], 1.0);

}

VECTOR4D Sampler::texture2D_TileData_Single(float u, float v)
{
	int uInt = u * (width - 1);
	int vInt = v * (height - 1);

	int tileX = uInt / tileW;
	int tileY = vInt / tileH;

	int inTileX = uInt % tileW;
	int inTileY = vInt % tileH;

	int index = ((tileY * widthInTiles + tileX) * (tileW * tileH)
		+ inTileY * tileW + inTileX)*channels;

	return VECTOR4D(imgData[index], 0.0, 0.0, 0.0);
}

//Texture2D直接采样
VECTOR4D Sampler::texture2D_Nearest(float s, float t)
{
	VECTOR4D color(1, 1, 1, 1);
	float u = (float)(width - 1) * s;
	float v = (float)(height - 1)*(1.0-t);
	int iu = (int)u;
	int iv = (int)v;
	int index = iu * width + iv;
	color.x = imgData[index * 3] * INV_SCALE;
	color.y = imgData[index * 3+1] * INV_SCALE;
	color.z = imgData[index * 3+2] * INV_SCALE;

	return color;
}

//将FB渲染到Texture: 将fb当中的数值写入sampler当中。
void writeFrameBuffer2Sampler(FrameBuffer * fb, Sampler * sampler)
{
	for (int i = 0; i < fb->width; ++i)
	{
		for (int j = 0; j < fb->height; ++j)
		{
			int index = (i * fb->width + j) * 3;
			sampler->imgData[index] = fb->c_data[index];
			sampler->imgData[index+1] = fb->c_data[index+1];
			sampler->imgData[index+2] = fb->c_data[index+2];
		}
	}
}
