#include "Scene.h"

#include <deque>
#include <fstream>
#include "RectangularPrism.h"
#include "RayTracer.h"

SceneMachine::SceneMachine()
{
	// testing
	Scene	scene;
	scene.name = "fuck";
	scene.floors = { {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}, {101,102,103,104,105,106,107,108,109,110,111,112,113,114,115} };
	mConfig.scenes.insert({ scene.name, scene });
	mConfig.Save();

	Scene	load;
	mConfig.scenes.clear();	// to ensure cereal writes correctly
	mConfig.Load();
	load.name = mConfig.filename;	std::cout << load.name << std::endl;
	load.floors = mConfig.scenes[load.name.c_str()].floors;
	for (int i = 0; i < scene.floors.size(); i++)
	{
		for (int j = 0; j < scene.floors[i].size(); j++)
			std::cout << scene.floors[i][j] << " ";
		std::cout << std::endl;
	}
}

SceneMachine::~SceneMachine()
{
	mFloor.DeleteAllInstances();
}

void SceneMachine::SetScene()
{
	Instance* instance = mFloor.CreateInstance();
	mFloor.MoveInstance(instance, q3Vec3(0,0,-10));
	mFloor.MoveInstance(instance, q3Vec3(0, 0, -10));
}

void SceneMachine::Render()
{
	for (std::deque<Instance*>::iterator it = mFloor.mInstances.begin(); it != mFloor.mInstances.end(); ++it)
		mFloor.DrawInstance(*it);
}

Instance* SceneMachine::PickInstance(glm::vec3 ray_origin, glm::vec3 ray_direction)
{
	RayTracer::Ray ray(ray_origin, ray_direction);
	for (std::deque<Instance*>::iterator it = mFloor.mInstances.begin(); it != mFloor.mInstances.end(); ++it)
	{
		//for (int i = 0; i < 4; ++i)
		//{
		//	for (int j = 0; j < 4; ++j)
		//		std::cout << (*it)->transform[i][j] << "\t";
		//	std::cout << std::endl;
		//}
		if (RayTracer::RayBoxCollide(ray, glm::vec3(-0.5, -0.5, -0.001), glm::vec3(0.5, 0.5, 0.001), (*it)->transform) == true)
		{
			std::cout << "hit" << std::endl;
		}
	}
	return nullptr;
}







void SceneConfigFile::Save()
{
	std::ofstream os(filename, std::ios::binary);
	cereal::BinaryOutputArchive archive(os);
	archive(scenes);
}

void SceneConfigFile::Load()
{
	std::ifstream is(filename, std::ios::binary);
	cereal::BinaryInputArchive archive(is);
	archive(scenes);
	//std::remove(mConfig.configFilename);
}