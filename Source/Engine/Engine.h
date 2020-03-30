#ifndef ENGINE_H
#define ENGINE_H

#include "SceneManager.h"
#include "RenderManager.h"
#include "DisplayManager.h"

class Engine
{
public:
	Engine();
	~Engine();

	bool startUp(HWND& hWnd);
	void shutDown(HWND& hWnd);

	void frame();

	void Resize(int width, int height);
public:

	SceneManager  engine_sceneManager;
	RenderManager engine_renderManager;
	DisplayManager engine_displayManager;
};



#endif