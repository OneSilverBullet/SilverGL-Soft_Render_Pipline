#include "Model.h"

void Model::update()
{
	mBounds.update(mModelMatrix);
}

Mesh * Model::getMesh()
{
	return &mMesh;
}

MATRIX4X4 * Model::getModelMatrix()
{
	return &mModelMatrix;
}

AABox * Model::getBounds()
{
	return &mBounds;
}

Texture * Model::getAlbedo()
{
	return &mAlbedo;
}

Texture * Model::getNormal()
{
	return &mNormal;
}

Texture * Model::getAO()
{
	return &mAmbient;
}

Texture * Model::getRoughness()
{
	return &mRoughness;
}

Texture * Model::getMetallic()
{
	return &mMetallic;
}
