#pragma once
#ifndef SCENE
#define SCENE

#include <vector>
#include "Floor.h"

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>

struct Scene
{
	std::string name;
	std::vector<std::vector<float> > floors;
private:
	friend class cereal::access;
	template<class Archive> void save(Archive & ar) const
	{
		ar(name, floors);
	}
	template<class Archive> void load(Archive & ar)
	{
		ar(name, floors);
	}
};

#include <map>
struct SceneConfigFile
{
	const std::string filename = "scenes.dat";
	std::map<std::string, Scene> scenes;
	void Save();
	void Load();
private:
	friend class cereal::access;
	template<class Archive> void save(Archive & ar) const
	{
		ar(scenes);
	}
	template<class Archive> void load(Archive & ar)
	{
		ar(scenes);
	}
};

class SceneMachine
{
public:
	SceneConfigFile	mConfig;
	Scene			mCurrentScene;
	Floor			mFloor;
	Instance*		mActiveInstance;

	SceneMachine();
	~SceneMachine();
	void SetScene();	// modify later to switch scene
	void Render();
	Instance* PickInstance(glm::vec3, glm::vec3);	// get by ray in world space
};

#endif SCENE