#include "softwareRender.h"

#include "Shader.h"

SoftwareRender::SoftwareRender()
{
}

SoftwareRender::~SoftwareRender()
{
}

bool SoftwareRender::startUp(int w, int h)
{
	//software_camera = new Camera();
	software_camera->BuildProjectMatrix(w, h);
	SilverGL::initFixFace();
	initKeys();
	return true;
}

void SoftwareRender::shutDown()
{
	releaseKeys();
	SilverGL::releaseFixFace();
	renderTarget = nullptr;
	software_camera = nullptr;
	//releaseDevice2Buf(&frontBuffer, &backBuffer, &depthBuffer);
}


//���Ƶ���ģ��
void SoftwareRender::drawTriangularMesh(Model * currentModel)
{
	Mesh* triMesh = currentModel->getMesh();
	std::vector<VECTOR3I>* pos_indices = &triMesh->fVertexIndices;
	std::vector<VECTOR3I>* uv_indices = &triMesh->fTextureIndices;
	std::vector<VECTOR3I>* nor_indices = &triMesh->fNormalsIndices;
	std::vector<VECTOR3D>* fNormals = &triMesh->fNormals;

	std::vector<VECTOR3D>* pos = &triMesh->vertices;
	std::vector<VECTOR3D>* uvs = &triMesh->texels;
	std::vector<VECTOR3D>* normals = &triMesh->normals;
	std::vector<VECTOR3D>* tangents = &triMesh->tangents;
	int numFaces = triMesh->numFaces;


	MATRIX4X4 worldToObject = (*(currentModel->getModelMatrix())).GetInverseTranspose();

	//��ʼ��Shader��Ϣ
	software_camera->BuildViewMatrix();
	viewMatrix = software_camera->camera_viewMatrix;
	modelMatrix = *currentModel->getModelMatrix();
	projectMatrix = software_camera->camera_projectMatrix;
	curTexture = currentModel->getAlbedo()->sampler;
	
	PBRShader pbrShader;
	//1. ��ʼ��ͼ��
	pbrShader.albedo = currentModel->getAlbedo();
	pbrShader.normal = currentModel->getNormal();
	pbrShader.ambientO = currentModel->getAO();
	pbrShader.roughness = currentModel->getRoughness();
	pbrShader.metal = currentModel->getMetallic();
	//2. ��ʼ���ƹ�
	VECTOR3D* lightPositions = new VECTOR3D[software_lightnum];
	VECTOR3D* lightColors = new VECTOR3D[software_lightnum];
	for (int i = 0; i < software_lightnum; ++i)
	{
		lightColors[i] = software_lights[i].color;
		lightPositions[i] = software_lights[i].position;
	}
	pbrShader.lightCol = lightColors;
	pbrShader.lightPos = lightPositions;
	pbrShader.cameraPos = software_camera->getPosition();
	pbrShader.numLights = software_lightnum;
	//3. ��ʼ����Ҫ����
	software_camera->BuildViewMatrix();
	pbrShader.MV = (software_camera->camera_viewMatrix)*(*currentModel->getModelMatrix());
	pbrShader.MVP = software_camera->camera_projectMatrix * pbrShader.MV;
	pbrShader.V = software_camera->camera_viewMatrix;
	pbrShader.M = *currentModel->getModelMatrix();
	pbrShader.N = (pbrShader.M.GetInverseTranspose());
	pbrShader.N.Transpose();
	

	for (int j = 0; j < numFaces; ++j)
	{
		//��ʼ����ǰģ�͵�j����������Ϣ��λ�ã����ߣ���������
		VECTOR3D trianglePrim[3], normalPrim[3], uvPrim[3], tangentPrim[3];
		//��ȡ���ģ�͵�j��������index��
		VECTOR3I f = (*pos_indices)[j];
		VECTOR3I n = (*nor_indices)[j];
		VECTOR3I u = (*uv_indices)[j];

		VECTOR4D* lightDirs = new VECTOR4D[software_lightnum * 3];
		pbrShader.lightDirVal = lightDirs;
		//��ȡ��ǰ�����ε�����������Ϣ�������������Ϣ
		packDataIntoTris(f, *pos, trianglePrim);
		packDataIntoTris(n, *normals, normalPrim);
		packDataIntoTris(u, *uvs, uvPrim);
		packDataIntoTris(f, *tangents, tangentPrim);
		VECTOR4D firtsPoint = VECTOR4D(trianglePrim[0]);
		//��鵱ǰ���Ƿ����������ӽǵģ������ɸ����
		VECTOR4D camPos = software_camera->getPosition();
		if (SilverGL::cullFace((*fNormals)[j], firtsPoint, worldToObject, camPos, CULL_BACK))
		{
			continue;
		}
		//����������
		Vertex modelA(trianglePrim[0], normalPrim[0], uvPrim[0], tangentPrim[0]);
		Vertex modelB(trianglePrim[1], normalPrim[1], uvPrim[1], tangentPrim[1]);
		Vertex modelC(trianglePrim[2], normalPrim[2], uvPrim[2], tangentPrim[2]);
		Triangle triangle(modelA, modelB, modelC);
		//���Ƶ�ǰ������
		SilverGL::drawFace(renderTarget->front, renderTarget->depth, pbrShader, &triangle);

		delete[] lightDirs;
	}

	delete[] lightPositions;
	delete[] lightColors;
}

FrameBuffer * SoftwareRender::getRenderTarget()
{
	return renderTarget->front;
}

void SoftwareRender::clearBuffers()
{
	SilverGL::clearScreen(renderTarget->front, 50, 50, 50);
	SilverGL::clearDepth(renderTarget->depth);
}

void SoftwareRender::setRenderMainCamera(Camera * camera)
{
	software_camera = camera;
}

void SoftwareRender::setRenderTarget(DisplayManager * target)
{
	renderTarget = target;
}

void SoftwareRender::setSceneLights(Light * lights, int numLight)
{
	software_lightnum = numLight;
	software_lights = lights;
}

void SoftwareRender::resize(int w, int h)
{
	software_camera->BuildProjectMatrix(w, h);
}

bool SoftwareRender::createDevice(int w, int h)
{
	return false;
}

void SoftwareRender::packDataIntoTris(VECTOR3I & index, std::vector<VECTOR3D> & vals, VECTOR3D * tris)
{
	tris[0] = vals[index.x];
	tris[1] = vals[index.y];
	tris[2] = vals[index.z];
}
