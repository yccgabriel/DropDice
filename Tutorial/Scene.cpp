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
	//mFloor.TranslateInstance(instance, q3Vec3(0, 0, -10));
}

void SceneMachine::Render()
{
	for (std::deque<Instance*>::iterator it = mFloor.mInstances.begin(); it != mFloor.mInstances.end(); ++it)
		mFloor.DrawInstance(*it);

	// draw Cubic Cross
	if (mActiveInstance != nullptr)
		mRubiksCore.Draw();
}

Instance* SceneMachine::PickNearestInstance(glm::vec3 ray_origin, glm::vec3 ray_direction)
{
	RayTracer::Ray ray(ray_origin, ray_direction);
	std::vector<Instance*> skewer;
	for (std::deque<Instance*>::iterator it = mFloor.mInstances.begin(); it != mFloor.mInstances.end(); ++it)		// Scan Floors
	{
		if (RayTracer::RayBoxCollide(ray, (*it)->mBoxMinXYZ , (*it)->mBoxMaxXYZ, (*it)->transform) == true)
			skewer.push_back(*it);
	}
	std::vector<Instance*> rods = {mRubiksCore.mXRod, mRubiksCore.mYRod, mRubiksCore.mZRod};
	for (std::vector<Instance*>::iterator it = rods.begin(); it != rods.end(); ++it)								// Scan Rods
	{
		if (RayTracer::RayBoxCollide(ray, (*it)->mBoxMinXYZ, (*it)->mBoxMaxXYZ, (*it)->transform) == true) 
			skewer.push_back(*it);
	}
	// find min distance of camera and object
	Instance*	nearestInstance = nullptr;
	double		nearestDistance = std::numeric_limits<double>::max();
	glm::vec4 camPos = glm::vec4(ray_origin, 1);
	for (std::vector<Instance*>::iterator it = skewer.begin(); it != skewer.end(); ++it)
	{
		double distance = glm::distance(camPos, ((*it)->transform)*glm::vec4(0, 0, 0, 1));
		if (distance < nearestDistance)
		{
			nearestDistance = distance;
			nearestInstance = *it;
		}
	}
	// set instance active flag for drawing and other action
	if (nearestInstance != nullptr)
	{
		bool nearestInstanceState = nearestInstance->mActive;
		for (std::deque<Instance*>::iterator it = mFloor.mInstances.begin(); it != mFloor.mInstances.end(); ++it)
			(*it)->mActive = false;
		nearestInstance->mActive = !nearestInstanceState;
		nearestInstance->mActive ? mActiveInstance = nearestInstance : mActiveInstance = nullptr;		// set mActiveInstance
		if (nearestInstance->mActive)	mRubiksCore.transform = nearestInstance->transform;				// move the rubik's core to the floor
	}
	return nearestInstance;
}

int SceneMachine::ClassifyInstance(Instance* instance)
{
	if (std::find(mRubiksCore.rod.mInstances.begin(), mRubiksCore.rod.mInstances.end(), instance) != mRubiksCore.rod.mInstances.end())
		return SceneMachine::ROD;
	else if (std::find(mFloor.mInstances.begin(), mFloor.mInstances.end(), instance) != mFloor.mInstances.end())
		return SceneMachine::FLOOR;
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