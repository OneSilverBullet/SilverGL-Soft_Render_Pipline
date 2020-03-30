#ifndef  FACE_H
#define FACE_H

#include "BaseDataSturcture.h"
#include "Maths/Maths.h"

class Triangle
{
public:
	Vertex modelA, modelB, modelC;
	VertexOut clipA, clipB, clipC;
	VECTOR2D ndcA, ndcB, ndcC;
	MATRIX4X4 clipMatrixInv;

	Triangle(Vertex a, Vertex b, Vertex c);
	Triangle();
	~Triangle();
	void Copy2Triangle(Vertex a, Vertex b, Vertex c);
	void Copy2TriangleOut(VertexOut a, VertexOut b, VertexOut c);
	void CalculateNDC();
	void CalculateClipMatrixInv();


};



#endif FACE_H