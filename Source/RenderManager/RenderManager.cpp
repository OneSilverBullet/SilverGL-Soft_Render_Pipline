#include "RenderManager.h"

RenderManager::RenderManager()
{
}

RenderManager::~RenderManager()
{
}

bool RenderManager::startUp(SceneManager& sceneManager, DisplayManager& displayManager)
{
	sceneLoader = &sceneManager;
	displayer = &displayManager;
	renderInstance.setRenderMainCamera(sceneLoader->getCurrentScene()->getMainCamera());
	
	if(!initSoftwareRender())
	{
		std::cout << "Error: failed to init software render" << std::endl;
		return false;
	}
	return true;
}

void RenderManager::shutDown()
{
	sceneLoader = nullptr;
	displayer = nullptr;
	renderInstance.shutDown();
}

void RenderManager::render()
{
	buildRenderQueue();
	renderInstance.clearBuffers();
	while (!renderObjectQueue->empty())
	{
		renderInstance.drawTriangularMesh(renderObjectQueue->front());
		renderObjectQueue->pop();
	}
	displayer->swapBuffers();
	renderInstance.setRenderMainCamera(nullptr);
}

void RenderManager::resize(int w, int h)
{
	renderInstance.resize(w, h);
}

void RenderManager::buildRenderQueue()
{
	Scene* currentScene = sceneLoader->getCurrentScene();
	renderInstance.setRenderMainCamera(currentScene->getMainCamera());
	renderInstance.setSceneLights(currentScene->getCurrentLights(), currentScene->getLightCount());
	renderInstance.setRenderTarget(displayer);
	renderObjectQueue = currentScene->getVisiblemodels();
}

bool RenderManager::initSoftwareRender()
{
	return renderInstance.startUp(SCREEN_WIDTH, SCREEN_HEIGHT);
}
