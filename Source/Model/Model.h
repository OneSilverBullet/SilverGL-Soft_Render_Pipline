#ifndef MODEL_H
#define MODEL_H

#include "Header.h"
#include "Geometry.h"
#include "Maths/Maths.h"
#include "Texture.h"
#include "Transformation.h"
#include "OBJParser.h"

class Model
{
public:
	Model(const Transformation& initParameters, const std::string meshPath, const std::string materialPath)
		:mAlbedo(materialPath + "_albedo.png", "RGB"),
		mNormal(materialPath + "_normal.png", "XYZ"),
		mAmbient(materialPath + "_ao.png", "BW"),
		mRoughness(materialPath + "_rough.png", "BW"),
		mMetallic(materialPath+"_metal.png", "BW"),
		mModelMatrix(initParameters.transform_model_matrix)
	{
		OBJ::buildMeshFromFile(mMesh, meshPath);
		mBounds.buildAABB(mMesh);
		mMesh.buildFaceNormals();
		mMesh.buildTangentSpace();
	}

	void update();
	Mesh *getMesh();
	MATRIX4X4 *getModelMatrix();
	AABox *getBounds();
	Texture *getAlbedo();
	Texture *getNormal();
	Texture *getAO();
	Texture *getRoughness();
	Texture *getMetallic();


private:
	//PBR材质
	Texture mAlbedo;
	Texture mNormal;
	Texture mAmbient;
	Texture mRoughness;
	Texture mMetallic;

	Mesh mMesh; //当前物体的面片
	AABox mBounds; //当前物体的包围盒
	MATRIX4X4 mModelMatrix; //当前物体在世界坐标系的位置




};



#endif