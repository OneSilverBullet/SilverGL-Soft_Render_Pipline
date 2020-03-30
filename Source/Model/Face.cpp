#include "Face.h"

Triangle::Triangle(Vertex a, Vertex b, Vertex c)
{
	modelA = a;
	modelB = b;
	modelC = c;
	clipMatrixInv.LoadIdentity();
}

Triangle::Triangle()
{
	clipMatrixInv.LoadIdentity();
}

Triangle::~Triangle()
{
}

void Triangle::Copy2Triangle(Vertex a, Vertex b, Vertex c)
{
	VertexCopy(&modelA, &a);
	VertexCopy(&modelB, &b);
	VertexCopy(&modelC, &c);
}

void Triangle::Copy2TriangleOut(VertexOut a, VertexOut b, VertexOut c)
{
	VertexOutCopy(&clipA, &a);
	VertexOutCopy(&clipB, &b);
	VertexOutCopy(&clipC, &c);
}

void Triangle::CalculateNDC()
{
	float invClipAW = 1.0 / clipA.w;
	float invClipBW = 1.0 / clipB.w;
	float invClipCW = 1.0 / clipC.w;
	ndcA.x = clipA.x * invClipAW;
	ndcA.y = clipA.y * invClipAW;
	ndcB.x = clipB.x * invClipBW;
	ndcB.y = clipB.y * invClipBW;
	ndcC.x = clipC.x * invClipCW;
	ndcC.y = clipC.y * invClipCW;

}


//设置相应的裁剪空间中的值
void Triangle::CalculateClipMatrixInv()
{
	clipMatrixInv.LoadIdentity();
	clipMatrixInv.entries[0] = clipA.x;
	clipMatrixInv.entries[1] = clipA.y;
	clipMatrixInv.entries[2] = clipA.w;
	clipMatrixInv.entries[4] = clipB.x;
	clipMatrixInv.entries[5] = clipB.y;
	clipMatrixInv.entries[6] = clipB.w;
	clipMatrixInv.entries[8] = clipC.x;
	clipMatrixInv.entries[9] = clipC.y;
	clipMatrixInv.entries[10] = clipC.w;
	clipMatrixInv.Invert();
}
