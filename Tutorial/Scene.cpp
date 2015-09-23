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
	Instance<RectangularPrism>* instance = mFloor.CreateInstance();
	mFloor.MoveInstance(instance, q3Vec3(0,0,-10));
	//mFloor.MoveInstance(instance, q3Vec3(0, 0, -10));
}

void Scene::Render()
{
	for (std::deque<Instance<RectangularPrism>*>::iterator it = mFloor.mInstances.begin(); it != mFloor.mInstances.end(); ++it)
		mFloor.DrawInstance(*it);
}