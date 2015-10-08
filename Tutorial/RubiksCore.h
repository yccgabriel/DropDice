#pragma once
#ifndef RUBIKSCENTERPIECE_H
#define RUBIKSCENTERPIECE_H

#include "Rod.h"
//#include "Cube.h"

class RubiksCore
{
public:
	Rod			rod;
	Instance	*mXRod, *mYRod, *mZRod;
	glm::mat4	transform;

	RubiksCore();
	~RubiksCore();
	void Draw();
};

#endif // RUBIKSCENTERPIECE_H