#include "Scene.h"
#include "OBJParser.h"
#include "Constants.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>


Scene::Scene(const std::string & sceneName)
{
	std::string folderPath = "scenes/" + sceneName;
	if (!findSceneFolder(folderPath))
	{
		emptyScene = true;
		std::cout << "Report: Load empty scene" << std::endl;
	}
	else
	{
		//加载摄像机灯光，模型
		emptyScene = !loadContent(folderPath, sceneName);
	}
}

Scene::~Scene()
{
	if (!emptyScene)
	{
		for (Model* x : models)
		{
			delete x;
		}
		delete[] lights;
	}
}

void Scene::update(unsigned int deltaT)
{
	mainCamera->Update();
	for (int i = 0; i < lightCount; ++i)
	{
		lights[i].update(deltaT);
	}
	for (Model* x : models)
	{
		x->update();
	}
	frustrumCulling();
}

std::queue<Model*>* Scene::getVisiblemodels()
{
	return &visibleModels;
}

Camera * Scene::getMainCamera()
{
	return mainCamera;
}

Light * Scene::getCurrentLights()
{
	return lights;
}

int Scene::getLightCount()
{
	return lightCount;
}

bool Scene::checkEmpty()
{
	return emptyScene;
}

bool Scene::loadContent(const std::string & baseFilePath, const std::string & sceneName)
{
	std::string configFilePath = baseFilePath + '/' + sceneName + "_config.txt";
	std::ifstream file(configFilePath.c_str());
	Transformation initParameters;
	if (!file.good())
	{
		std::cout << "Error : config " << sceneName << " is not exist!" << std::endl;
		return false;
	}
	else
	{
		std::string line, key, x, y, z;
		std::getline(file, line);
		std::istringstream iss(line);
		iss >> key;
		if (key != "s")
		{
			std::cout << "Error : config file " << sceneName << " have error format" << std::endl;
			return false;
		}
		else
		{
			while (!file.eof())
			{
				std::getline(file, line);
				std::istringstream iss(line);
				iss >> key;
				if (key == "m")
				{
					std::cout << "Loading models ... " << std::endl;
					iss >> key;
					int modelCount = stoi(key);
					std::string modelMeshID, modelMaterialID;
					for (int i = 0; i < modelCount; ++i)
					{
						//得到Model的mesh和material ID
						std::getline(file, line);
						std::istringstream modelData(line);
						modelData >> key >> modelMeshID >> modelMaterialID;
					
						//position
						std::getline(file, line);
						std::istringstream pos(line);
						pos >> key >> x >> y >> z;
						initParameters.Move(stof(x), stof(y), stof(z));

						//rotation
						std::getline(file, line);
						std::istringstream rot(line);
						rot >> key >> x >> y >> z;
						initParameters.Rotate(stof(x) * RADIUS, stof(y) * RADIUS, stof(z) * RADIUS);
						
						//scale
						std::getline(file, line);
						std::istringstream sca(line);
						sca >> key >> x >> y >> z;
						initParameters.Scale(stof(x));

						//读一个空行，然后构造模型
						std::getline(file, line);

						loadSceneModel(baseFilePath, initParameters, modelMeshID, modelMaterialID);
					}
				}
				else if(key == "l")
				{
					std::cout << "Loading Lights ..." << std::endl;
					iss >> key;
					lightCount = stoi(key);
					std::string lightType, radius, period;
					lights = new Light[lightCount];
					for (int i = 0; i < lightCount; ++i)
					{
						std::getline(file, line);
						std::istringstream lightData(line);
						lightData >> key >> lightType;
						if (lightType == "o") {
							lights[i].type = 'o';
							std::getline(file, line);
							std::istringstream orb(line);
							orb >> key >> radius >> period;
							lights[i].radius = stof(radius);
							lights[i].time /= stof(period) * 1000; //miliseconds
						}
						else if (lightType == "l") {
							lights[i].type = 'l';
							std::getline(file, line);
							std::istringstream orb(line);
							orb >> key >> radius >> period;
							lights[i].radius = stof(radius);
							lights[i].time /= stof(period) * 1000;
						}
						else if (lightType == "c") {
							lights[i].type = 'c';
						}
						else if (lightType == "f") {
							lights[i].type = 'f';
						}

						//Position
						std::getline(file, line);
						std::istringstream pos(line);
						pos >> key >> x >> y >> z;
						lights[i].position = VECTOR3D(stof(x), stof(y), stof(z));

						//Color
						std::getline(file, line);
						std::istringstream col(line);
						col >> key >> x >> y >> z;
						lights[i].color = VECTOR3D(stof(x), stof(y), stof(z));

						//
						std::getline(file, line);
					}
				}
			}
		}
	}
	mainCamera = new Camera();
	return !models.empty();
}

bool Scene::findSceneFolder(const std::string & scenePath)
{
	struct stat info;
	if (stat(scenePath.c_str(), &info) != 0)
	{
		std::cout << "Error :the scene path cant be accessed! " << std::endl;
		return false;
	}
	if (info.st_mode & S_IFDIR)
	{
		std::cout << "Report : the scene path is valid! " << std::endl;
		return true;
	}
	std::cout << "Error : the scene path is not exists" << std::endl;
	return false;
}

void Scene::loadSceneModel(const std::string & baseFilePath, const Transformation & init, const std::string modelMeshID, const std::string modelMaterialID)
{
	std::string meshFilePath = baseFilePath + "/meshes/" + modelMeshID + "_mesh.obj";
	if (!OBJ::fileExists(meshFilePath))
	{
		std::cout << "Error : the mesh " << modelMeshID << " doesnt exist!" << std::endl;
		return;
	}
	else
	{
		std::cout << "Report : load mesh " << modelMeshID << std::endl;
		std::string materialPath = baseFilePath + "/materials/" + modelMaterialID + "/" + modelMaterialID;
		models.push_back(new Model(init, meshFilePath, materialPath));
	}
}

void Scene::frustrumCulling()
{
	for (Model* x : models)
	{
		bool visible = mainCamera->checkVisible(x->getBounds());
		if (visible)
		{
			visibleModels.push(x);
		}
	}
}
