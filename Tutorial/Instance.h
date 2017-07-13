#pragma once
#ifndef INSTANCE
#define INSTANCE

#include <glm/glm.hpp>
#include <qu3e/q3.h>

class Instance
{
public:
	q3Vec3 mSize;			// control the object size, [x, y, z]
	glm::mat4 localtrans;	// transform made within object space.
	glm::mat4 transform;	// transform to model space
	glm::vec3 mBoxMinXYZ;
	glm::vec3 mBoxMaxXYZ;
	float lambda;			// the fraction [0,1] indicate ratio of boxMax-boxMin
	q3Body* mBody;
	bool mReady;			// flag indicate instance is completely created to draw
	bool mActive = false;	// flag indicate instance is clicked and chosen

	Instance();
	~Instance();
	void ConvertTransform();// convert q3transform to glm::mat4 transform
};

#endif // INSTANCE