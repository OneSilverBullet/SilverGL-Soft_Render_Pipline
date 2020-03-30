#ifndef SOFTWARERENDER_H
#define SOFTWARERENDER_H

#include "Header.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "GraphicLibs.h"
#include "DisplayManager.h"

class SoftwareRender
{
public:
	SoftwareRender();
	~SoftwareRender();

	bool startUp(int w, int h);
	void shutDown();
	void drawTriangularMesh(Model* currentModel);
	FrameBuffer* getRenderTarget();
	void clearBuffers();

	void setRenderMainCamera(Camera* camera);
	void setRenderTarget(DisplayManager* target);
	void setSceneLights(Light* lights, int numLight);
	void resize(int w, int h);

private:
	bool createDevice(int w, int h);
	void packDataIntoTris(VECTOR3I& index, std::vector<VECTOR3D>& vals, VECTOR3D* tris);


	Camera* software_camera;
	bool startUpComplete = false;

	int software_lightnum;
	Light* software_lights;

	DisplayManager* renderTarget;

	//FrameBuffer* pixelBuffer;
	//DepthBuffer* depthBuffer;

};



#endif