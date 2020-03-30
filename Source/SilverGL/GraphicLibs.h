#ifndef GRAPHICLIBS_H
#define GRAPHICLIBS_H

#include "Face.h"
#include "Header.h"
#include "AdvancedShader.h"


//��ǰ��������Ļ��Ⱦ�洢����ֱ����Ӧ�ó����ͼ���豸��������
namespace SilverGL
{
	extern float eyex, eyey, eyez, clipNear;
	extern bool BlendFlag;
	extern Triangle* cull_face1;
	extern Triangle* cull_face2;

	void  initFrameBuffer(FrameBuffer** pfb, int width, int height);
	void releaseFrameBuffer(FrameBuffer** pfb);
	void initDepthBuffer(DepthBuffer** db, int width, int height);
	void releaseDepthBuffer(DepthBuffer** db);
	void initDevice(FrameBuffer** fb, DepthBuffer** db, int width, int height);
	void releaseDevice(FrameBuffer** fb, DepthBuffer** db);
	void initDevice2Buf(FrameBuffer** fb1, FrameBuffer**fb2, DepthBuffer** db, FrameBuffer** f1, FrameBuffer** f2, int width, int height);
	void releaseDevice2Buf(FrameBuffer** fb1, FrameBuffer** fb2, DepthBuffer** db, FrameBuffer** f1, FrameBuffer** f2);

	//֡�������
	void clearScreen(FrameBuffer* fb, unsigned char red, unsigned char green, unsigned char yellow);
	void clearScreenFast(FrameBuffer* fb, unsigned char color);
	void clearDepth(DepthBuffer* db);

	//֡����ϸ�ڲ���
	void drawFrameBuffer(FrameBuffer* fb, int x, int y,
		unsigned char red, unsigned char green, unsigned char blue);
	void readFrameBuffer(FrameBuffer* fb, int x, int y,
		unsigned char& red, unsigned char& green, unsigned char& blue);
	void writeDepth(DepthBuffer* db, int x, int y, float depth);
	float readDepth(DepthBuffer* db, int x, int y);

	//��Ⱦ������ϸ����
	bool cullFace(Triangle* face, int flag);
	bool cullFace(VECTOR3D& faceNormal, VECTOR4D& vertex, MATRIX4X4& worldToObject, VECTOR4D cameraPos, int flag);


	int gammaAdjust(float n);
	void viewPortTransform(float ndcX, float ndcY, float width, float height,
		int& screenX, int& screenY);
	void viewPortTransform(float ndcX, float ndcY, float width, float height,
		float& screenX, float& screenY);
	void invViewPortTransform(float screenX, float screenY, float width, float height,
		float & ndcX, float& ndcY);
	void viewportTransform(FrameBuffer * fb, VECTOR4D *vertices);

	float edge(VECTOR4D &a, VECTOR4D &b, VECTOR4D &c);
	void triBoundBox(int &xMax, int &xMin, int &yMax, int &yMin, VECTOR4D *vertices, FrameBuffer *pixelBuffer);

	//�Լ�����Ĺ�դ��
	void rasterize(FrameBuffer* fb, DepthBuffer* db, FragmentShader fs, Triangle* face);
	void rasterize2(FrameBuffer* fb, DepthBuffer* db, FragmentShader fs, Triangle* face);
	//�Ը���ģ�͵Ĺ�դ��
	void rasterize_advance(FrameBuffer * fb, DepthBuffer * db, OriginShader& shader, VECTOR4D  * face, VECTOR3D* uvs);
	void rasterize_advance_op(FrameBuffer * fb, DepthBuffer * db, OriginShader& shader, VECTOR4D * clipVertex);

	void blend(unsigned char srcR, unsigned char srcG, unsigned char srcB, unsigned char srcA,
		unsigned char dstR, unsigned char dstG, unsigned char dstB,
		unsigned char& finalR, unsigned char& finalG, unsigned char& finalB);

	//��Ⱦ���̷�װ
	void drawFace(FrameBuffer* fb, DepthBuffer* db, OriginShader& shader,
		Triangle* face);//�߼�shader�����
	void drawFace(FrameBuffer* fb, DepthBuffer* db, VertexShader vs,
		FragmentShader fs, int cullFlag, Triangle* face); //����shader�����W
	void drawFaces(FrameBuffer* fb, DepthBuffer* db, VertexShader vs,
		FragmentShader fs, int cullFlag, Vertex* buffer, int count);


	//�ü���Near���ӿ�ǰ���������
	void initFixFace();
	void releaseFixFace();

	int check(Triangle* face);
	void fixFaces(Triangle* face, int fixFlag);
	void fix1FailFace(VertexOut fail, VertexOut succ1, VertexOut succ2);
	void fix2FailFace(VertexOut fail1, VertexOut fail2, VertexOut succ);
	void interpolate2v(float pa, float pb, VertexOut a, VertexOut b, VertexOut& result);
	void scaleColor(float red, float green, float blue, unsigned char& iRed, unsigned char& iGreen, unsigned char& iBlue);
}


#endif