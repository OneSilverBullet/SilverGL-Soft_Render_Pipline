#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Texture::Texture(std::string src, std::string type)
{
	//����BMP�ļ�
	if (type == "BMP")
	{
		//1. ��ͼ����ص�unsigned char����
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
		//2. ��loader���е����ݿ�����sampler
		memcpy(sampler->imgData, loader->data, loader->width * loader->height * 3 * sizeof(unsigned char));
		delete loader;
	}
	else//ʹ��stb_image���ж�ȡͼ������
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
