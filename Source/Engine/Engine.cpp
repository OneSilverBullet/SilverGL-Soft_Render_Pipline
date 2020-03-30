#include "Engine.h"

#include <ctime>

Engine::Engine()
{
}

Engine::~Engine()
{
}

bool Engine::startUp(HWND& hWnd)
{
	if (!engine_sceneManager.startUp())
	{
		std::cout << "Error: engine scene manager init failed" << std::endl;
		return false;
	}
	if (!engine_displayManager.startUp(hWnd))
	{
		std::cout << "Error: engine display manager init failed" << std::endl;
		return false;
	}
	if (!engine_renderManager.startUp(engine_sceneManager, engine_displayManager))
	{
		std::cout << "Error:engine render manager init failed" << std::endl;
		return false;
	}
	return true;
}

void Engine::shutDown(HWND& hWnd)
{
	engine_renderManager.shutDown();
	engine_sceneManager.shutDown();
	engine_displayManager.shutDown(hWnd);
}

void Engine::frame()
{
	std::cout << "Report: frame render loop start" << std::endl;
	//std::cout << "Report: scene manager update(0/2)" << std::endl;
	engine_sceneManager.update(1000);
	double start, end;
	start = clock();
	//std::cout << "Report: render manager update(1/2)" << std::endl;
	engine_renderManager.render();
	//std::cout << "Report: frame render loop end(2/2)" << std::endl;
	end = clock();
	std::cout << "Cost : " << (end - start) / CLOCKS_PER_SEC << std::endl;
	engine_displayManager.frame();
}


void Engine::Resize(int width, int height)
{
	engine_displayManager.releaseBuffer();
	engine_displayManager.initBuffers();
	engine_displayManager.releaseDIB();
	engine_displayManager.initDIB();
	engine_renderManager.resize(width, height);
}
