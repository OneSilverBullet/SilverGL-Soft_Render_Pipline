#ifndef SCENE_H
#define SCENE_H

#include "Header.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class Scene
{
public:
	Scene(const std::string& sceneFolder);
	~Scene();
	//¸üÐÂ
	void update(unsigned int deltaT);

	std::queue<Model*>* getVisiblemodels();
	Camera* getMainCamera();
	Light* getCurrentLights();
	int getLightCount();
	bool checkEmpty();

private:
	bool emptyScene;
	Camera* mainCamera;
	int lightCount;
	Light* lights;

	std::queue<Model*> visibleModels;
	std::vector<Model*> models;

	bool loadContent(const std::string & baseFilePath, const std::string& sceneName);
	bool findSceneFolder(const std::string & scenePath);
	void loadSceneModel(const std::string & baseFilePath, const Transformation& init,
		const std::string modelMeshID, const std::string modelMaterialID);
	void frustrumCulling();

};




#endif