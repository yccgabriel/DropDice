#include "RubiksCore.h"



RubiksCore::RubiksCore()
{
	mXRod = rod.CreateInstance();
	mYRod = rod.CreateInstance();
	mZRod = rod.CreateInstance();
	
	rod.RotateInstanceInLocalSpace(mXRod, glm::mat4());
	rod.RotateInstanceInLocalSpace(mYRod, glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));	// rotate 90 degrees about Z-axis
	rod.RotateInstanceInLocalSpace(mZRod, glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f)));	// rotate 90 degrees about negative Y-axis
}

RubiksCore::~RubiksCore()
{
}

void RubiksCore::Draw()
{
	mXRod->transform = transform*mXRod->localtrans;
	mYRod->transform = transform*mYRod->localtrans;
	mZRod->transform = transform*mZRod->localtrans;
	rod.DrawInstance(mXRod, glm::vec3(1.0f, 0.0f, 0.0f));	// red
	rod.DrawInstance(mYRod, glm::vec3(0.0f, 1.0f, 0.0f));	// green
	rod.DrawInstance(mZRod, glm::vec3(0.0f, 0.0f, 1.0f));	// blue
}