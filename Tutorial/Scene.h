#pragma once
#ifndef SCENE
#define SCENE

#include "Floor.h"

class Scene
{
public:
	Floor mFloor;

	Scene();
	~Scene();
	void SetScene();	// modify later to switch scene
	void Render();
};

#endif SCENE