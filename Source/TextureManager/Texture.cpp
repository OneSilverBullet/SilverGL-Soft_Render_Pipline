#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture(std::string src, std::string type)
{
	//加载BMP文件
	if (type == "BMP")
	{
		//1. 将图像加载到unsigned char当中
		BmpLoader* loader = new BmpLoader();
		if (!loader->loadBitmap(src.c_str()))
		{
			sampler = NULL;
			return;
		}
		width = loader->width;
		height = loader->height;
		channels = 3;
		sampler = new Sampler(loader->width, loader->height, 3);
		//2. 将loader当中的内容拷贝到sampler
		memcpy(sampler->imgData, loader->data, loader->width * loader->height * 3 * sizeof(unsigned char));
		delete loader;
	}
	else//使用stb_image进行读取图像数据
	{
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(src.c_str(), &width, &height, &channels, 0);
		
		if (data)
		{
			sampler = new Sampler(width, height, channels);
			if (type == "RGB")
			{
				for (int i = 0; i < width*height*channels; ++i) {
					sampler->imgData[i] = pow((float)data[i] * (1 / 255.0f), 2.2);
				}
				sampler->tileData();
			}
			else if (type == "XYZ")
			{
				for (int i = 0; i < width*height*channels; ++i) {
					sampler->imgData[i] = (float)data[i] * (2 / 255.0f) - 1.0f;
				}
				sampler->tileData();
			}
			else if (type == "BW")
			{
				for (int i = 0; i < width*height*channels; ++i) {
					sampler->imgData[i] = (float)data[i] * (1 / 255.0f);
				}
				sampler->tileData();
			}
			else
			{
				std::cout << "Error: unrecognized texture format" << std::endl;
			}
		}
	}
}

Texture::~Texture()
{
	if (sampler != NULL)
	{
		delete sampler;
	}
}

VECTOR4D Texture::texture2D(float s, float t)
{
	return sampler->texture2D(s, t);
}
