#include "SceneManager.h"

SceneManager::SceneManager()
{
	
}

SceneManager::~SceneManager()
{
}

bool SceneManager::startUp()
{
	currentSceneID = "chest";
	if (!loadScene(currentSceneID))
	{
		std::cout << "Could not load scene. No model succesfully loaded!" << std::endl;
		return false;
	}
	return true;
}

void SceneManager::shutDown()
{
	delete currentScene;
}

bool SceneManager::switchScene(std::string sceneID)
{
	if (currentSceneID == sceneID)
	{
		std::cout << "Warning : scene id is  the same! " << std::endl;
	}
	else
	{
		currentSceneID = sceneID;
		delete currentScene;
		return loadScene(sceneID);
	}
	return false;
}

void SceneManager::update(unsigned int deltaT)
{
	currentScene->update(deltaT);
}

Scene * SceneManager::getCurrentScene()
{
	return currentScene;
}

bool SceneManager::loadScene(std::string sceneID)
{
	std::cout << "Report : load scene of " << sceneID << std::endl;
	currentScene = new Scene(sceneID);
	return !currentScene->checkEmpty();
}
