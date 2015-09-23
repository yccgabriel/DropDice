#include "Scene.h"

#include <deque>

Scene::Scene()
{
}

Scene::~Scene()
{
	mFloor.DeleteAllInstances();
}

void Scene::SetScene()
{
	mFloor.CreateInstance();
}

void Scene::Render()
{
	for (std::deque<Instance*>::iterator it = mFloor.mInstances.begin(); it != mFloor.mInstances.end(); ++it)
		mFloor.DrawInstance(*it);
}