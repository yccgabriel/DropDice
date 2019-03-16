#pragma once
#ifndef RUBIKSCENTERPIECE_H
#define RUBIKSCENTERPIECE_H

#include "Rod.h"
#include "RayTracer.h"
//#include "Cube.h"

class RubiksCore
{
public:
	bool		mTranslateCore = false;
	bool		mRotateCore = false;
	Rod			rod;
	Instance	*mXRod, *mYRod, *mZRod;
	RT::Line	mActiveRodAxis;
	Instance*	mAttachedInstance;			// the Floor instance the the Rubiks core attached to
	Instance*	mActiveInstance;
	int			mActiveInstanceType;
	glm::vec3	mReferenceLambdaPoint;		// the lambda point when cursor clicked on the rod/cube
	glm::vec3	mReferenceAttachedInstancePosition;	// the position of attached instance before mouse release
	enum InstanceType { NTH, ROD, CUBE };
	glm::mat4	transform;

	RubiksCore();
	~RubiksCore();
	void Draw();
	void ClickOnRod(Instance*, RayTracer::Line);
	void ClickOnCube(Instance*, RayTracer::Line);
	void Release();
	void TranslateCore(const glm::vec3&, const glm::vec3&);
};

#endif // RUBIKSCENTERPIECE_H