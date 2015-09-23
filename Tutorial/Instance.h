#pragma once
#ifndef INSTANCE
#define INSTANCE

#include <glm/glm.hpp>
#include <qu3e/q3.h>

template <typename T>
class Instance
{
public:
	glm::mat4 transform;
	q3Body* mBody;
	bool mReady;			// flag indicate instance is completely created to draw

	Instance();
	~Instance();
	void SetTransform();	// convert q3transform to glm::mat4 transform
};

#endif // INSTANCE