#include "Instance.h"
#include <glm/gtx/transform.hpp>


Instance::Instance()
{
	mSize = q3Vec3(1.0f, 1.0f, 1.0f);
	mReady = false;
}

Instance::~Instance()
{
}

void Instance::SetTransform()
{
	const q3Transform& q3tran = mBody->GetTransform();
	const q3Vec3& t = q3tran.position;
	const q3Mat3& r = q3tran.rotation;
	glm::mat4	rot(r.ex.x, r.ex.y, r.ex.z, 0,
		r.ey.x, r.ey.y, r.ey.z, 0,
		r.ez.x, r.ez.y, r.ez.z, 0,
		0, 0, 0, 1);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(t.x, t.y, t.z));
	transform = trans * rot;

	// debugging
	//std::cout << r.ex.x << "\t" << r.ex.y << "\t" << r.ex.z << std::endl;
	//std::cout << r.ey.x << "\t" << r.ey.y << "\t" << r.ey.z << std::endl;
	//std::cout << r.ez.x << "\t" << r.ez.y << "\t" << r.ez.z << std::endl;
	//std::cout << std::endl;
}