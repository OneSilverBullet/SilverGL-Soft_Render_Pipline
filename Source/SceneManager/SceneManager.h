#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

//用于场景管理，对Scene的封装
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	bool startUp();
	void shutDown();

	bool switchScene(std::string sceneID);
	void update(unsigned int deltaT);

	Scene* getCurrentScene();

private:
	bool loadScene(std::string sceneID);
	std::string currentSceneID;
	Scene* currentScene;
};


#endif // 
