#ifdef _DEBUG
#include <iostream>
#endif
#include "RayTracer.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// source: http://blogs.aerys.in/jeanmarc-leroux/2012/05/25/optimized-aabb-ray-intersection/
bool RayTracer::RayBoxCollide(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax, const glm::mat4& model)
{
	glm::mat4 invModel = glm::inverse(model);
	Ray	localRay(invModel*ray.position, ray.direction);
	float ox = localRay.position.x;
	float oy = localRay.position.y;
	float oz = localRay.position.z;

	localRay.direction = glm::normalize(localRay.direction);
	float dx = 1.0f / localRay.direction.x;
	float dy = 1.0f / localRay.direction.y;
	float dz = 1.0f / localRay.direction.z;

	float minX = boxMin.x;
	float minY = boxMin.y;
	float minZ = boxMin.z;
	float maxX = boxMax.x;
	float maxY = boxMax.y;
	float maxZ = boxMax.z;

	float tx1 = (minX - ox) * dx;
	float tx2 = (maxX - ox) * dx;

	float min = tx1 < tx2 ? tx1 : tx2;
	float max = tx1 > tx2 ? tx1 : tx2;
	float tmin = min;
	float tmax = max;

	float ty1 = (minY - oy) * dy;
	float ty2 = (maxY - oy) * dy;

	min = ty1 < ty2 ? ty1 : ty2;
	max = ty1 > ty2 ? ty1 : ty2;
	tmin = tmin > min ? tmin : min;
	tmax = tmax < max ? tmax : max;

	float tz1 = (minZ - oz) * dz;
	float tz2 = (maxZ - oz) * dz;
	
	min = tz1 < tz2 ? tz1 : tz2;
	max = tz1 > tz2 ? tz1 : tz2;
	tmin = tmin > min ? tmin : min;
	tmax = tmax < max ? tmax : max;

	return tmax >= std::max(0.0f, tmin) && tmin < std::numeric_limits<int>::max();
}