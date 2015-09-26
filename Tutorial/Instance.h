#pragma once
#ifndef INSTANCE
#define INSTANCE

#include <glm/glm.hpp>
#include <qu3e/q3.h>

class Instance
{
public:
	q3Vec3 mSize;			// control the object size, [x, y, z]
	glm::mat4 transform;
	q3Body* mBody;
	bool mReady;			// flag indicate instance is completely created to draw

	Instance();
	~Instance();
	void SetTransform();	// convert q3transform to glm::mat4 transform
};

#endif // INSTANCE