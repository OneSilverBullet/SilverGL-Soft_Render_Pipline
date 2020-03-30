#include "GraphicLibs.h"
#include <iostream>
#include <fstream>
#include <algorithm>


float SilverGL::eyex, SilverGL::eyey, SilverGL::eyez, SilverGL::clipNear;

//GL 状态
bool SilverGL::BlendFlag = false;

//临时变量
Triangle* SilverGL::cull_face1;
Triangle* SilverGL::cull_face2;


const int gammaTable[256] = { 0, 21, 28, 34, 39, 43, 46,
		50, 53, 56, 59, 61, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84,
		85, 87, 89, 90, 92, 93, 95, 96, 98, 99, 101, 102, 103, 105, 106,
		107, 109, 110, 111, 112, 114, 115, 116, 117, 118, 119, 120, 122,
		123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
		136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147,
		148, 149, 150, 151, 151, 152, 153, 154, 155, 156, 156, 157, 158,
		159, 160, 160, 161, 162, 163, 164, 164, 165, 166, 167, 167, 168,
		169, 170, 170, 171, 172, 173, 173, 174, 175, 175, 176, 177, 178,
		178, 179, 180, 180, 181, 182, 182, 183, 184, 184, 185, 186, 186,
		187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195,
		195, 196, 197, 197, 198, 199, 199, 200, 200, 201, 202, 202, 203,
		203, 204, 205, 205, 206, 206, 207, 207, 208, 209, 209, 210, 210,
		211, 212, 212, 213, 213, 214, 214, 215, 215, 216, 217, 217, 218,
		218, 219, 219, 220, 220, 221, 221, 222, 223, 223, 224, 224, 225,
		225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231,
		232, 232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238,
		238, 239, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244,
		245, 245, 246, 246, 247, 247, 248, 248, 249, 249, 249, 250, 250,
		251, 251, 252, 252, 253, 253, 254, 254, 255, 255 };


void SilverGL::initFrameBuffer(FrameBuffer ** pfb, int width, int height)
{
	*pfb = (FrameBuffer *)malloc(sizeof(FrameBuffer));
	(*pfb)->height = height;
	(*pfb)->width = width;
	(*pfb)->c_data = new unsigned char[3 * width*height];
	memset((*pfb)->c_data, 0, sizeof(unsigned char) * width * height * 3);
}

void SilverGL::releaseFrameBuffer(FrameBuffer ** pfb)
{
	if (*pfb != NULL)
	{
		delete[](*pfb)->c_data;
		free(*pfb);
		*pfb = NULL;
	}
	return;
}

void SilverGL::initDepthBuffer(DepthBuffer ** db, int width, int height)
{
	*db = (DepthBuffer*)malloc(sizeof(DepthBuffer));
	(*db)->height = height;
	(*db)->width = width;
	(*db)->d_data = new float[width * height * 1];
	memset((*db)->d_data, 0, sizeof(float)*width * height * 1);
}

void SilverGL::releaseDepthBuffer(DepthBuffer ** db)
{
	if (*db != NULL)
	{
		delete[](*db)->d_data;
		free(*db);
		(*db) = NULL;
	}
	return;
}

void SilverGL::initDevice(FrameBuffer ** fb, DepthBuffer ** db, int width, int height)
{
	initFrameBuffer(fb, width, height);
	initDepthBuffer(db, width, height);
}

void SilverGL::releaseDevice(FrameBuffer ** fb, DepthBuffer ** db)
{
	releaseFrameBuffer(fb);
	releaseDepthBuffer(db);
}

void SilverGL::initDevice2Buf(FrameBuffer ** fb1, FrameBuffer ** fb2, DepthBuffer ** db, FrameBuffer** f1, FrameBuffer** f2, int width, int height)
{
	initFrameBuffer(fb1, width, height);
	initFrameBuffer(fb2, width, height);
	initDepthBuffer(db, width, height);
	*f1 = *fb1;
	*f2 = *fb2;
}

void SilverGL::releaseDevice2Buf(FrameBuffer ** fb1, FrameBuffer ** fb2, DepthBuffer ** db,  FrameBuffer** f1, FrameBuffer** f2)
{
	releaseFrameBuffer(fb1);
	releaseFrameBuffer(fb2);
	releaseDepthBuffer(db);
	*f1 = NULL;
	*f2 = NULL;
}

void SilverGL::clearScreen(FrameBuffer * fb, unsigned char red, unsigned char green, unsigned char blue)
{
	for (int i = 0; i < fb->height; ++i)
	{
		for (int j = 0; j < fb->width; ++j)
		{
			int index = (i * fb->width + j)*3;
			fb->c_data[index] = red;
			fb->c_data[index+1] = green;
			fb->c_data[index + 2] = blue;
		}
	}
}

void SilverGL::clearScreenFast(FrameBuffer * fb, unsigned char color)
{
	int size = fb->width *fb->height * 3;
	memset(fb->c_data, color * sizeof(unsigned char), size);
}

void SilverGL::clearDepth(DepthBuffer * db)
{
	for (int i = 0; i < db->height; ++i)
	{
		for (int j = 0; j < db->width; ++j)
		{
			db->d_data[i*db->width + j] = -2.0;
		}
	}
}

void convertToScreen(int height, int& sx, int& sy)
{
	sy = height - 1 - sy;
}

float clamp(float val, float x, float y)
{
	if (val <= x)
		return x;
	if (val >= y)
		return y;
	return val;
}

int SilverGL::gammaAdjust(float n) {
	int val = (int)(clamp(n * 255, 0, 255) + 0.5f);
	return gammaTable[val];
}


void SilverGL::drawFrameBuffer(FrameBuffer * fb, int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
	convertToScreen(fb->height, x, y);
	int index = (y*fb->width + x) * 3;
	fb->c_data[index] = red;
	fb->c_data[index + 1] = green;
	fb->c_data[index + 2] = blue;
}

void SilverGL::readFrameBuffer(FrameBuffer * fb, int x, int y, unsigned char & red, unsigned char & green, unsigned char & blue)
{
	convertToScreen(fb->height, x, y);
	int index = (y*fb->width + x) * 3;
	red = fb->c_data[index];
	green = fb->c_data[index + 1];
	blue = fb->c_data[index + 2];
}

void SilverGL::writeDepth(DepthBuffer * db, int x, int y, float depth)
{
	convertToScreen(db->height, x, y);
	db->d_data[y*db->width + x] = depth;
}

float SilverGL::readDepth(DepthBuffer * db, int x, int y)
{
	convertToScreen(db->height, x, y);
	return db->d_data[y*db->width + x];
}

//针对复杂模型面
bool SilverGL::cullFace(VECTOR3D& faceNormal, VECTOR4D& vertex, MATRIX4X4& worldToObject, VECTOR4D cameraPos, int flag)
{
	VECTOR4D viewDir = worldToObject * (cameraPos)-vertex;
	float intensity = faceNormal.DotProduct(viewDir);
	if (flag == CULL_NONE)
		return false;
	else if (flag == CULL_BACK)
		return intensity <= 0.0;
	else if (flag == CULL_FRONT)
		return intensity > 0.0;
	return false;
}

//针对普通面的裁剪
bool SilverGL::cullFace(Triangle * face, int flag)
{
	VECTOR3D faceNormal(face->clipA.nx, face->clipA.ny, face->clipA.nz);
	VECTOR3D eyeVec(eyex - face->clipA.wx, eyey - face->clipA.wy, eyez - face->clipA.wz);
	if (flag == CULL_NONE)
		return false;
	if (flag == CULL_BACK) {
		if (eyeVec.DotProduct(faceNormal) <= 0)
			return true;
		return false;
	}
	else if (flag == CULL_FRONT) {
		if (eyeVec.DotProduct(faceNormal) >= 0)
			return true;
		return false;
	}
	return false;
}

//ndc坐标：-1~1
//将NDC坐标转换到ViewPort坐标
void SilverGL::viewPortTransform(float ndcX, float ndcY, float width, float height, int & screenX, int & screenY)
{
	float biasX = (ndcX + 1.0) * 0.5f;
	float biasY = (ndcY + 1.0) * 0.5f;
	screenX = roundf(biasX * (width - 1.0));
	screenY = roundf(biasY *(height - 1.0));
}

void SilverGL::viewPortTransform(float ndcX, float ndcY, float width, float height, float & screenX, float & screenY)
{
	float biasX = (ndcX + 1.0) * 0.5;
	float biasY = (ndcY + 1.0) * 0.5;
	screenX = biasX * (width - 1);
	screenY = biasY * (height - 1);
}

void SilverGL::invViewPortTransform(float screenX, float screenY, float width, float height, float & ndcX, float & ndcY)
{
	float scaleX = (float)screenX / (width - 1.0);
	float scaleY = (float)screenY / (height - 1.0);
	ndcX = scaleX * 2.0 - 1.0;
	ndcY = scaleY * 2.0 - 1.0;
}

void SilverGL::viewportTransform(FrameBuffer * fb, VECTOR4D *vertices) {
	for (int i = 0; i < 3; ++i) {
		//Adding half a pixel to avoid gaps on small vertex values
		//将从-1~1的裁剪坐标系转换到对应的0-h/w的视口坐标系
		vertices[i].x = ((vertices[i].x + 1) * fb->width * 0.5) + 0.5;
		vertices[i].y = ((vertices[i].y + 1) * fb->height * 0.5) + 0.5;
	}
}

//光栅化阶段
//简单光栅化
void SilverGL::rasterize(FrameBuffer * fb, DepthBuffer * db, FragmentShader fs, Triangle * face)
{
	float ndcX = 0, ndcY = 0, clipW = 0;
	int scrAX, scrAY, scrBX, scrBY, scrCX, scrCY;
	//将三角形的三个顶点的NDC转换到ViewPort坐标系下
	viewPortTransform(face->ndcA.x, face->ndcA.y, fb->width, fb->height, scrAX, scrAY);
	viewPortTransform(face->ndcB.x, face->ndcB.y, fb->width, fb->height, scrBX, scrBY);
	viewPortTransform(face->ndcC.x, face->ndcC.y, fb->width, fb->height, scrCX, scrCY);

	//得到三角形的最大XY，最小XY，得到包围盒
	int minX = max(0, min(scrAX, min(scrBX, scrCX)));
	int maxX = min(fb->width - 1, max(scrAX, max(scrBX, scrCX)));
	int minY = max(0, min(scrAY, min(scrBY, scrCY)));
	int maxY = min(fb->height - 1, max(scrAY, max(scrBY, scrCY)));

	for (int scrY = minY; scrY <= maxY; scrY++)
	{
		for (int scrX = minX; scrX <= maxX; scrX++)
		{
			invViewPortTransform(scrX, scrY, fb->width, fb->height, ndcX, ndcY);
			VECTOR4D ndcPixel(ndcX, ndcY, 1, 0);

			VECTOR4D proportion4D = face->clipMatrixInv * ndcPixel;
			VECTOR3D proportionFragment(proportion4D.x, proportion4D.y, proportion4D.z);
			float pa = proportionFragment.x;
			float pb = proportionFragment.y;
			float pc = proportionFragment.z;
			if (pa < 0.0 || pb < 0.0 || pc < 0.0)
			{
				continue; //说明当前的顶点不在三角形内部
			}
			float sum = pa + pb + pc;
			float invSum = 1 / sum;
			pa *= invSum;
			pb *= invSum;
			pc *= invSum;

			Fragment frag;
			interpolate3f(pa, pb, pc, face->clipA.w, face->clipB.w, face->clipC.w, clipW);
			interpolate3f(pa, pb, pc, face->clipA.z, face->clipB.z, face->clipC.z, frag.ndcz);
			
			float invClipW = 1.0 / clipW;
			frag.ndcz *= invClipW;
			if (frag.ndcz < -1.0 || frag.ndcz > 1.0) //检查当前的片元是否在NDC允许的空间中。
			{
				continue;
			}

			//Z-BUFFER算法，对比深度，如果深度当前小于目前的ndcz（因为摄像机方向指向负方向）
			//那么就写入当前depth buffer。
			if (db != NULL)
			{
				float storeZ = readDepth(db, scrX, scrY);
				if (storeZ < frag.ndcz)
				{
					continue;
				}
				writeDepth(db, scrX, scrY, frag.ndcz);
			}
			//插值得到当前片段对应的NDC坐标X,Y
			interpolate3f(pa, pb, pc, face->clipA.x, face->clipB.x, face->clipC.x, frag.ndcx);
			frag.ndcx *= invClipW;
			interpolate3f(pa, pb, pc, face->clipA.y, face->clipB.y, face->clipC.y, frag.ndcy);
			frag.ndcy *= invClipW;

			//插值得到当前片段对应的世界坐标系
			interpolate3f(pa, pb, pc, face->clipA.wx, face->clipB.wx, face->clipC.wx, frag.wx);
			interpolate3f(pa, pb, pc, face->clipA.wy, face->clipB.wy, face->clipC.wy, frag.wy);
			interpolate3f(pa, pb, pc, face->clipA.wz, face->clipB.wz, face->clipC.wz, frag.wz);
			interpolate3f(pa, pb, pc, face->clipA.ww, face->clipB.ww, face->clipC.ww, frag.ww);

			//插值得到当前片段对应的法线向量
			interpolate3f(pa, pb, pc, face->clipA.nx, face->clipB.nx, face->clipC.nx, frag.nx);
			interpolate3f(pa, pb, pc, face->clipA.ny, face->clipB.ny, face->clipC.ny, frag.ny);
			interpolate3f(pa, pb, pc, face->clipA.nz, face->clipB.nz, face->clipC.nz, frag.nz);

			//插值得到当前片段对应的纹理坐标
			interpolate3f(pa, pb, pc, face->clipA.s, face->clipB.s, face->clipC.s, frag.s);
			interpolate3f(pa, pb, pc, face->clipA.t, face->clipB.t, face->clipC.t, frag.t);

			FragmentOut outFrag;
			fs(frag, outFrag);
			unsigned char cr = 255, cg = 255, cb = 255;
			scaleColor(outFrag.r, outFrag.g, outFrag.b, cr, cg, cb);
			drawFrameBuffer(fb, scrX, scrY, cr, cg, cb);
		}
	}
}


//扫描线算法，得到scrY扫面线截断的x1,x2.
void ScanLine(float scrAX, float scrAY, float scrBX, float scrBY, float scrCX, float scrCY,
	float scrY, float& x1, float& x2)
{
	if (scrAY == scrBY && scrBY == scrCY)
	{
		return;
	}
	if (scrAY == scrBY)
	{
		float paramAC = (scrY - scrAY) / (scrCY - scrAY);
		float paramBC = (scrY - scrBY) / (scrCY - scrBY);
		x1 = (scrCX - scrAX)* paramAC;
		x2 = (scrCX - scrBX) * paramBC;
	}
	else if (scrAY == scrCY)
	{
		float paramAB = (scrY - scrAY) / (scrBY - scrAY);
		float paramBC = (scrY - scrBY) / (scrCY - scrBY);
		x1 = (scrBX - scrAX)*paramAB + scrAX;
		x2 = (scrCX - scrBX)*paramBC + scrBX;
	}
	else if (scrBY == scrCY)
	{
		float paramAB = (scrY - scrAY) / (scrBY - scrAY);
		float paramAC = (scrY - scrAY) / (scrCY - scrAY);
		x1 = (scrBX - scrAX) * paramAB + scrAX;
		x2 = (scrCX - scrAX) * paramAC + scrAX;
	}
	else
	{
		float paramAB = (scrY - scrAY) / (scrBY - scrAY);
		float paramAC = (scrY - scrAY) / (scrCY - scrAY);
		float paramBC = (scrY - scrBY) / (scrCY - scrBY);
		bool ab = (paramAB <= 1.0 && paramAB >= 0.0) ? true : false;
		bool ac = (paramAC <= 1.0 && paramAC >= 0.0) ? true : false;
		bool bc = (paramBC <= 1.0 && paramBC >= 0.0) ? true : false;
		float xAB = (scrBX - scrAX) * paramAB + scrAX;
		float xAC = (scrCX - scrAX) * paramAC + scrAX;
		float xBC = (scrCX - scrBX) * paramBC + scrBX;
		if (!ab)
		{
			x1 = xAC;
			x2 = xBC;
		}
		else if (!ac)
		{
			x1 = xAB;
			x2 = xBC;
		}
		else if (!bc)
		{
			x1 = xAB;
			x2 = xAC;
		}
		else
		{
			x1 = xAC;
			x2 = xBC;
			if (x1 == x2)
				x2 = xAB;
		}
	}
}

//经过优化的简单光栅化
void SilverGL::rasterize2(FrameBuffer * fb, DepthBuffer * db, FragmentShader fs, Triangle * face)
{
	float ndcX = 0, ndcY = 0, clipW = 0;
	int scrAX, scrAY, scrBX, scrBY, scrCX, scrCY;
	//将三角形的三个顶点的NDC转换到ViewPort坐标系下
	viewPortTransform(face->ndcA.x, face->ndcA.y, fb->width, fb->height, scrAX, scrAY);
	viewPortTransform(face->ndcB.x, face->ndcB.y, fb->width, fb->height, scrBX, scrBY);
	viewPortTransform(face->ndcC.x, face->ndcC.y, fb->width, fb->height, scrCX, scrCY);
	if (scrAY == scrBY && scrAY == scrCY)
		return;
	//得到三角形的最大XY，最小XY，得到包围盒
	int minY = max(0, min(scrAY, min(scrBY, scrCY)));
	int maxY = min(fb->height - 1, max(scrAY, max(scrBY, scrCY)));

	for (int scrY = minY; scrY <= maxY; ++scrY)
	{
		float x1, x2;
		ScanLine(scrAX, scrAY, scrBX, scrBY, scrCX, scrCY, (float)scrY, x1, x2);
		int minX = max(0, roundf(min(x1, x2)));
		int maxX = min(fb->width - 1, roundf(max(x1, x2)));

		for (int scrX = minX; scrX <= maxX; ++scrX)
		{
			invViewPortTransform(scrX, scrY, fb->width, fb->height, ndcX, ndcY);
			VECTOR4D ndcPixel(ndcX, ndcY, 1, 0);
			VECTOR4D proportion4D = face->clipMatrixInv * ndcPixel;
			VECTOR3D proportionFragment(proportion4D.x, proportion4D.y, proportion4D.z);
			float pa = proportionFragment.x;
			float pb = proportionFragment.y;
			float pc = proportionFragment.z;
			if (pa < 0.0 || pb < 0.0 || pc < 0.0)
			{
				continue; //说明当前的顶点不在三角形内部
			}
			float sum = pa + pb + pc;
			float invSum = 1 / sum;
			pa *= invSum;
			pb *= invSum;
			pc *= invSum;

			Fragment frag;
			interpolate3f(pa, pb, pc, face->clipA.w, face->clipB.w, face->clipC.w, clipW);
			interpolate3f(pa, pb, pc, face->clipA.z, face->clipB.z, face->clipC.z, frag.ndcz);

			float invClipW = 1.0 / clipW;
			frag.ndcz *= invClipW;
			if (frag.ndcz < -1.0 || frag.ndcz > 1.0) //检查当前的片元是否在NDC允许的空间中。
			{
				continue;
			}

			//Z-BUFFER算法，对比深度，如果深度当前小于目前的ndcz（因为摄像机方向指向负方向）
			//那么就写入当前depth buffer。
			if (db != NULL)
			{
				float storeZ = readDepth(db, scrX, scrY);
				if (storeZ < frag.ndcz)
				{
					continue;
				}
				writeDepth(db, scrX, scrY, frag.ndcz);
			}
			//插值得到当前片段对应的NDC坐标X,Y
			interpolate3f(pa, pb, pc, face->clipA.x, face->clipB.x, face->clipC.x, frag.ndcx);
			frag.ndcx *= invClipW;
			interpolate3f(pa, pb, pc, face->clipA.y, face->clipB.y, face->clipC.y, frag.ndcy);
			frag.ndcy *= invClipW;

			//插值得到当前片段对应的世界坐标系
			interpolate3f(pa, pb, pc, face->clipA.wx, face->clipB.wx, face->clipC.wx, frag.wx);
			interpolate3f(pa, pb, pc, face->clipA.wy, face->clipB.wy, face->clipC.wy, frag.wy);
			interpolate3f(pa, pb, pc, face->clipA.wz, face->clipB.wz, face->clipC.wz, frag.wz);
			interpolate3f(pa, pb, pc, face->clipA.ww, face->clipB.ww, face->clipC.ww, frag.ww);

			//插值得到当前片段对应的法线向量
			interpolate3f(pa, pb, pc, face->clipA.nx, face->clipB.nx, face->clipC.nx, frag.nx);
			interpolate3f(pa, pb, pc, face->clipA.ny, face->clipB.ny, face->clipC.ny, frag.ny);
			interpolate3f(pa, pb, pc, face->clipA.nz, face->clipB.nz, face->clipC.nz, frag.nz);

			//插值得到当前片段对应的纹理坐标
			interpolate3f(pa, pb, pc, face->clipA.s, face->clipB.s, face->clipC.s, frag.s);
			interpolate3f(pa, pb, pc, face->clipA.t, face->clipB.t, face->clipC.t, frag.t);

			FragmentOut outFrag;
			fs(frag, outFrag);
			unsigned char cr = 255, cg = 255, cb = 255;
			unsigned char sr = 255, sg = 255, sb = 255;
			scaleColor(outFrag.r, outFrag.g, outFrag.b, cr, cg, cb);
			//混合
			if (BlendFlag) //通过颜色混合来添加到FB上
			{
				readFrameBuffer(fb, scrX, scrY, sr, sg, sb);
				blend(cr, cg, cb, outFrag.a, sr, sg, sb, cr, cg, cb);
			}
			drawFrameBuffer(fb, scrX, scrY, cr, cg, cb);
		}
	}
}


//用于计算权重
float SilverGL::edge(VECTOR4D &a, VECTOR4D &b, VECTOR4D &c) {
	return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

void SilverGL::triBoundBox(int &xMax, int &xMin, int &yMax, int &yMin, VECTOR4D *vertices, FrameBuffer *pixelBuffer) {
	xMax = max(vertices[0].x, max(vertices[1].x, vertices[2].x ));
	xMin = min( vertices[0].x, min(vertices[1].x, vertices[2].x ));

	yMax = max( vertices[0].y, max( vertices[1].y, vertices[2].y ));
	yMin = min(vertices[0].y, min( vertices[1].y, vertices[2].y) );

	//Clip against screen
	xMax = min(xMax, pixelBuffer->width - 1);
	xMin = max(xMin, 0);

	yMax = min(yMax, pixelBuffer->height - 1);
	yMin = max(yMin, 0);
}


void SilverGL::rasterize_advance_op(FrameBuffer * fb, DepthBuffer * db, OriginShader& shader, VECTOR4D * clipVertex) {
	float area;
	VECTOR3D hW{ 1 / clipVertex[0].w, 1 / clipVertex[1].w, 1 / clipVertex[2].w }; //得到每一个顶点的w倒数
	//每一个片段的变量
	float depth, uPers, vPers, areaPers, count = 0; 
	VECTOR3D e, e_row, f;
	VECTOR3D rgbVals{ 255, 255, 255 };
	//将坐标转换到视口空间
	viewportTransform(fb, clipVertex);

	area = edge(clipVertex[0], clipVertex[1], clipVertex[2]);
	if (area <= 0) return;
	area = 1 / area;

	//求出三角形的包围盒
	int xMax, xMin, yMax, yMin;
	triBoundBox(xMax, xMin, yMax, yMin, clipVertex, fb);

	//每一个三角形边的坐标差
	VECTOR3D zVals{ clipVertex[0].z,clipVertex[1].z,clipVertex[2].z };
	float A01 = clipVertex[0].y - clipVertex[1].y, B01 = clipVertex[1].x - clipVertex[0].x;
	float A12 = clipVertex[1].y - clipVertex[2].y, B12 = clipVertex[2].x - clipVertex[1].x;
	float A20 = clipVertex[2].y - clipVertex[0].y, B20 = clipVertex[0].x - clipVertex[2].x;

	//计算当前点的权重Edge values 
	VECTOR4D point{ (float)xMin, (float)yMin, 0, 0 };
	e_row.x = edge(clipVertex[1], clipVertex[2], point);
	e_row.y = edge(clipVertex[2], clipVertex[0], point);
	e_row.z = edge(clipVertex[0], clipVertex[1], point);

	//Iterating through each pixel in triangle bounding box
	for (int y = yMin; y <= yMax; ++y) {
		//Bary coordinates at start of row
		e.x = e_row.x;
		e.y = e_row.y;
		e.z = e_row.z;
		for (int x = xMin; x <= xMax; ++x) {
			if ((e.x >= 0) && (e.y >= 0) && (e.z >= 0)) {
				//Zbuffer check
				depth = (e*area).DotProduct(zVals);
				if (db != NULL)
				{
					float storeZ = readDepth(db, x, y);
					if (storeZ < depth && depth < 1.0)
					{
						writeDepth(db, x, y, depth);
						//对重心坐标进行透视矫正
						f = e * hW;
						areaPers = 1.0 / (f.x + f.y + f.z);
						//评估当前u,v受到的影响
						uPers = f.y * areaPers;
						vPers = f.z * areaPers;

						//运行片元着色器，这里传入的是权重
						rgbVals = shader.FragmentShader(uPers, vPers);
						//unsigned char cr = 255, cg = 255, cb = 255;
						//scaleColor(rgbVals.x, rgbVals.y, rgbVals.z, cr, cg, cb);
						drawFrameBuffer(fb, x, y, gammaAdjust(rgbVals.x), gammaAdjust(rgbVals.y), gammaAdjust(rgbVals.z));
					}
				}
			}
			//One step to the right
			e.x += A12;
			e.y += A20;
			e.z += A01;
		}
		//One row step
		e_row.x += B12;
		e_row.y += B20;
		e_row.z += B01;
	}
}

void SilverGL::rasterize_advance(FrameBuffer * fb, DepthBuffer * db, OriginShader & shader, VECTOR4D * clipVertex, VECTOR3D * uvs)
{
	float area;
	VECTOR3D hW{ 1 / clipVertex[0].w, 1 / clipVertex[1].w, 1 / clipVertex[2].w }; //得到每一个顶点的w倒数
	//每一个片段的变量
	float depth, uPers, vPers, areaPers, count = 0; //u, v, 被透视矫正过
	VECTOR3D e, e_row, f;
	VECTOR3D rgbVals{ 255, 255, 255 };
	//将坐标转换到视口空间
	viewportTransform(fb, clipVertex);

	area = edge(clipVertex[0], clipVertex[1], clipVertex[2]);
	if (area <= 0) return;
	area = 1 / area;
	//求出三角形的包围盒
	int xMax, xMin, yMax, yMin;
	triBoundBox(xMax, xMin, yMax, yMin, clipVertex, fb);
	for (int y = yMin; y <= yMax; ++y)
	{
		float x1, x2;
		//扫描线算法求出当前三角形需要绘制的最小x和最大x
		ScanLine(clipVertex[0].x, clipVertex[0].y, clipVertex[1].x, clipVertex[1].y, clipVertex[2].x, clipVertex[2].y, (float)y, x1, x2);
		xMin = max(0, roundf(min(x1, x2)));
		xMax = min(fb->width - 1, roundf(max(x1, x2)));

		for (int x = xMin; x <= xMax; ++x)
		{
			//重心法求出每一个视口点的权重
			VECTOR4D point{ (float)x, (float)y, 0, 0 };
			float w0 = edge(clipVertex[1], clipVertex[2], point);
			float w1 = edge(clipVertex[2], clipVertex[0], point);
			float w2 = edge(clipVertex[0], clipVertex[1], point);
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
			{
				w0 *= area;
				w1 *= area;
				w2 *= area;
				VECTOR3D w(w0, w1, w2);
				VECTOR3D zVals1{ 1 / clipVertex[0].z, 1 / clipVertex[1].z, 1 / clipVertex[2].z };
				float depth1 = 1.0 / (w.DotProduct(zVals1));
				float storeZ = readDepth(db, x, y);
				if (storeZ < depth1 && depth1 < 1.0)
				{
					writeDepth(db, x, y, depth1);

					float interu = depth1 * (uvs[0].x * zVals1.x *w0 + uvs[1].x * zVals1.y *w1 + uvs[2].x * zVals1.z *w2);
					float interv = depth1 * (uvs[0].y * zVals1.x *w0 + uvs[1].y * zVals1.y *w1 + uvs[2].y * zVals1.z *w2);

					//运行片元着色器
					rgbVals = shader.FragmentShader(interu, interv);
					int resx = rgbVals.x;
					int resy = rgbVals.y;
					int resz = rgbVals.z;
					//unsigned char cr = 255, cg = 255, cb = 255;
					//scaleColor(rgbVals.x, rgbVals.y, rgbVals.z, cr, cg, cb);
					drawFrameBuffer(fb, x, y, rgbVals.x, rgbVals.y, rgbVals.z);
				}
			}
		}
	}
}

void SilverGL::blend(unsigned char srcR, unsigned char srcG, unsigned char srcB,  unsigned char srcA,
	unsigned char dstR, unsigned char dstG, unsigned char dstB, 
	unsigned char & finalR, unsigned char & finalG, unsigned char & finalB)
{
	float dstA = 1.0 - srcA;
	float finR = (float)srcR *srcA - (float)dstR *dstA;
	float finG = (float)srcG * srcA - (float)dstG *dstA;
	float finB = (float)srcB *srcA - (float)dstB * dstA;
	finalR = finR > 255 ? 255 : finR;
	finalG = finG > 255 ? 255 : finG;
	finalB = finB > 255 ? 255 : finB;
}

void SilverGL::drawFace(FrameBuffer * fb, DepthBuffer * db, OriginShader & shader,  Triangle * face)
{
	//1. 执行顶点着色器
	shader.VertexShader(face->modelA, 0, face->clipA);
	shader.VertexShader(face->modelB, 1, face->clipB);
	shader.VertexShader(face->modelC, 2, face->clipC);
	
	//2. 抽象出裁剪坐标点
	VECTOR4D verts[3];
	verts[0] = VECTOR4D(face->clipA.x, face->clipA.y, face->clipA.z, face->clipA.w);
	verts[1] = VECTOR4D(face->clipB.x, face->clipB.y, face->clipB.z, face->clipB.w);
	verts[2] = VECTOR4D(face->clipC.x, face->clipC.y, face->clipC.z, face->clipC.w);

	//3. 对裁剪坐标点进行透视除法
	for (int i = 0; i < 3; ++i) {
		verts[i].x /= verts[i].w;
		verts[i].y /= verts[i].w;
		verts[i].z /= verts[i].w;
	}

	//4. 检查当前面是否在视口内部
	int clipFlag = check(face);
	if (clipFlag != 111)
	{
		return;
	}
	else  if (clipFlag == 111)
	{
		//5. 执行光栅化，传入坐标点+UV，对两个属性进行插值
		VECTOR3D uvs[3];
		uvs[0] = VECTOR3D(face->modelA.s, face->modelA.t, 0.0);
		uvs[1] = VECTOR3D(face->modelB.s, face->modelB.t, 0.0);
		uvs[2] = VECTOR3D(face->modelC.s, face->modelC.t, 0.0);
		rasterize_advance_op(fb, db, shader, verts);
	}
}

void SilverGL::drawFace(FrameBuffer * fb, DepthBuffer * db, VertexShader vs, FragmentShader fs, int cullFlag, Triangle * face)
{
	vs(face->modelA, face->clipA);
	vs(face->modelB, face->clipB);
	vs(face->modelC, face->clipC);

	if (cullFace(face, cullFlag))
	{
		return;
	}
	int clipFlag = check(face);
	if(clipFlag != 111)
	{
		if (clipFlag == 000)
		{
			return;
		}
		fixFaces(face, clipFlag);
		if (!cullFace(cull_face1, cullFlag))
		{
			cull_face1->CalculateClipMatrixInv();
			cull_face1->CalculateNDC();
			rasterize2(fb, db, fs, cull_face1);
		}
		if (clipFlag == 101 || clipFlag == 110 || clipFlag == 011)
		{
			if (!cullFace(cull_face2, cullFlag))
			{
				cull_face2->CalculateClipMatrixInv();
				cull_face2->CalculateNDC();
				rasterize2(fb, db, fs, cull_face2);
			}
		}
	}
	else  if (clipFlag == 111) 
	{
		face->CalculateClipMatrixInv();
		face->CalculateNDC();
		rasterize2(fb, db, fs, face);
	}
}

void SilverGL::drawFaces(FrameBuffer * fb, DepthBuffer * db, VertexShader vs, FragmentShader fs, int cullFlag, Vertex * buffer, int count)
{
	for (int i = 0; i < count; ++i)
	{
		Triangle face(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]);
		drawFace(fb, db, vs, fs, cullFlag, &face);
	}
}

void SilverGL::initFixFace()
{
	cull_face1 = new Triangle();
	cull_face2 = new Triangle();
}

void SilverGL::releaseFixFace()
{
	delete cull_face1;
	delete cull_face2;
}

//检查当前三角形那些点是可见的，哪些点是不可见的
//生成三位数字来辅助说明哪些点是可见的哪些点是不可见的
int SilverGL::check(Triangle * face)
{
	bool failA = false, failB = false, failC = false;
	int nFail = 0;
	if (face->clipA.vz / face->clipA.vw > -clipNear) {
		failA = true;
		nFail++;
	}
	if (face->clipB.vz / face->clipB.vw > -clipNear) {
		failB = true;
		nFail++;
	}
	if (face->clipC.vz / face->clipC.vw > -clipNear) {
		failC = true;
		nFail++;
	}
	if (nFail == 3)
		return 000;
	else if (nFail == 0)
		return 111;
	else if (nFail == 2) {
		if (failA&&failB)
			return 001;
		else if (failA&&failC)
			return 010;
		else if (failB&&failC)
			return 100;
	}
	else if (nFail == 1) {
		if (failA)
			return 011;
		else if (failB)
			return 101;
		else if (failC)
			return 110;
	}
	return 000;
}

//依据三位数字fixFlag，判断哪些点是在NEAR后面的。
//依据具体的点来将face生成两个面cull_face1,cull_face2
void SilverGL::fixFaces(Triangle * face, int fixFlag)
{
	switch (fixFlag)
	{
	case 011:
		fix1FailFace(face->clipA, face->clipB, face->clipC);
		break;
	case 101:
		fix1FailFace(face->clipB, face->clipA, face->clipC);
		break;
	case 110:
		fix1FailFace(face->clipC, face->clipA, face->clipB);
		break;
	case 001:
		fix2FailFace(face->clipA, face->clipB, face->clipC);
		break;
	case 010:
		fix2FailFace(face->clipA, face->clipC, face->clipB);
		break;
	case 100:
		fix2FailFace(face->clipB, face->clipC, face->clipA);
		break;
	}
}

//依据两个顶点a,b,以及权值pa，pb，插值得到中间点result
void SilverGL::interpolate2v(float pa, float pb,
	VertexOut a, VertexOut b,
	VertexOut& result) {
	interpolate2f(pa, pb, a.x, b.x, result.x);
	interpolate2f(pa, pb, a.y, b.y, result.y);
	interpolate2f(pa, pb, a.z, b.z, result.z);
	interpolate2f(pa, pb, a.w, b.w, result.w);
	interpolate2f(pa, pb, a.wx, b.wx, result.wx);
	interpolate2f(pa, pb, a.wy, b.wy, result.wy);
	interpolate2f(pa, pb, a.wz, b.wz, result.wz);
	interpolate2f(pa, pb, a.ww, b.ww, result.ww);
	interpolate2f(pa, pb, a.vx, b.vx, result.vx);
	interpolate2f(pa, pb, a.vy, b.vy, result.vy);
	interpolate2f(pa, pb, a.vz, b.vz, result.vz);
	interpolate2f(pa, pb, a.vw, b.vw, result.vw);
	interpolate2f(pa, pb, a.nx, b.nx, result.nx);
	interpolate2f(pa, pb, a.ny, b.ny, result.ny);
	interpolate2f(pa, pb, a.nz, b.nz, result.nz);
	interpolate2f(pa, pb, a.s, b.s, result.s);
	interpolate2f(pa, pb, a.t, b.t, result.t);
}

//只有一个点不可见，此时可见面为一个四边形，生成两个三角形代替
void SilverGL::fix1FailFace(VertexOut fail, VertexOut succ1, VertexOut succ2)
{
	float z = -clipNear;
	float invFailW = 1.0 / fail.vw;
	float invSucc1W = 1.0 / succ1.vw;
	float invSucc2W = 1.0 / succ2.vw;
	VECTOR3D  pFail(fail.vx*invFailW, fail.vy * invFailW, fail.vz * invFailW);
	VECTOR3D  pSucc1(succ1.vx * invSucc1W, succ1.vy * invSucc1W, succ1.vz * invSucc1W);
	VECTOR3D  pSucc2(succ2.vx * invSucc2W, succ2.vy * invSucc2W, succ2.vz * invSucc2W);
	//1.对顶点1进行插值。
	float param1 = calcZPara(pFail.z, pSucc1.z, z);
	VECTOR3D interPoint1 = calcParaEqu(pFail, pSucc1, param1);//得到交点的坐标插值
	float sp = (pSucc1 - interPoint1).GetLength();
	float fp = (pFail - interPoint1).GetLength();
	float sum = sp + fp;
	float invSum = 1.0 / sum;
	sp *= invSum;
	fp *= invSum;
	VertexOut inter1;
	interpolate2v(sp, fp, succ1, fail, inter1);  //插值得到第一个交点的所有属性插值

	//对顶点2进行插值。
	float param2 = calcZPara(pFail.z, pSucc2.z, z);
	VECTOR3D interPoint2 = calcParaEqu(pFail, pSucc2, param2);
	sp = (pSucc2 - interPoint2).GetLength();
	fp = (pFail - interPoint2).GetLength();
	sum = sp + fp;
	invSum = 1.0 / sum;
	sp *= invSum;
	fp *= invSum;
	VertexOut inter2;
	interpolate2v(sp, fp, succ2, fail, inter2);

	//将在可见面的四边形部分转为两个三角形
	cull_face1->Copy2TriangleOut(succ1, inter1, inter2);
	cull_face2->Copy2TriangleOut(succ2, succ1, inter2);
}

//有两个点不可见，那么生成一个三角形来代替
void SilverGL::fix2FailFace(VertexOut fail1, VertexOut fail2, VertexOut succ)
{
	float z = -clipNear;
	float invFail1W = 1.0 / fail1.vw;
	float invFail2W = 1.0 / fail2.vw;
	float invSuccW = 1.0 / succ.vw;
	VECTOR3D pFail1(fail1.vx*invFail1W, fail1.vy * invFail1W, fail1.vz * invFail1W);
	VECTOR3D pFail2(fail2.vx*invFail2W, fail2.vy * invFail2W, fail2.vz * invFail2W);
	VECTOR3D pSucc(succ.vx * invSuccW, succ.vy*invSuccW, succ.vz * invSuccW);
	float param1 = calcZPara(pFail1.z, pSucc.z, z);
	VECTOR3D interPoint1 = calcParaEqu(pFail1, pSucc, param1);
	float sp = (pSucc - interPoint1).GetLength();
	float fp = (pFail1 - interPoint1).GetLength();
	float sum = sp + fp;
	float invSum = 1.0 / sum;
	sp *= invSum;
	fp *= invSum;
	VertexOut inter1;
	interpolate2v(sp, fp, succ, fail1, inter1);

	float param2 = calcZPara(pFail2.z, pSucc.z, z);
	VECTOR3D interPoint2 = calcParaEqu(pFail2, pSucc, param2);
	sp = (pSucc - interPoint2).GetLength();
	fp = (pFail2 - interPoint2).GetLength();
	sum = sp + fp;
	invSum = 1.0 / sum;
	sp *= invSum;
	fp *= invSum;
	VertexOut inter2;
	interpolate2v(sp, fp, succ, fail2, inter2);

	cull_face1->Copy2TriangleOut(succ, inter1, inter2);
}

void SilverGL::scaleColor(float red, float green, float blue,
	unsigned char& iRed, unsigned char& iGreen, unsigned char& iBlue)
{
	red *= 255;
	green *= 255;
	blue *= 255;
	red = red > 255 ? 255 : red;
	green = green > 255 ? 255 : green;
	blue = blue > 255 ? 255 : blue;
	iRed = blue;
	iGreen = green;
	iBlue = red;
}
