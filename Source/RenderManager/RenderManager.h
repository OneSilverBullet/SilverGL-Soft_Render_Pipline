#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H
#include "SceneManager.h"
#include "softwareRender.h"
#include "DisplayManager.h"
#include "Header.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();
	bool startUp(SceneManager& sceneManager, DisplayManager& displayManager);
	void shutDown();
	void render();
	void resize(int w, int h);

private:
	void buildRenderQueue();
	bool initSoftwareRender();

	SceneManager* sceneLoader;
	DisplayManager* displayer;

	SoftwareRender renderInstance;
	std::queue<Model*>* renderObjectQueue;
};


#endif