#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"

//���ڳ���������Scene�ķ�װ
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
