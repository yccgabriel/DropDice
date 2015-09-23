#include "Scene.h"

#include <deque>
#include "RectangularPrism.h"

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
	for (std::deque<Instance<RectangularPrism>*>::iterator it = mFloor.mInstances.begin(); it != mFloor.mInstances.end(); ++it)
		mFloor.DrawInstance(*it);
}