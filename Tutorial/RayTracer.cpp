#ifdef _DEBUG
#include <iostream>
#endif
#include "RayTracer.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#ifdef _DEBUG
std::ostream& operator<<(std::ostream& out, const glm::mat4& m)
{
	out << m[0][0] << "\t" << m[0][1] << "\t" << m[0][2] << "\t" << m[0][3] << "\n";
	out << m[1][0] << "\t" << m[1][1] << "\t" << m[1][2] << "\t" << m[1][3] << "\n";
	out << m[2][0] << "\t" << m[2][1] << "\t" << m[2][2] << "\t" << m[2][3] << "\n";
	out << m[3][0] << "\t" << m[3][1] << "\t" << m[3][2] << "\t" << m[3][3] << "\n";
	return out;
}
#endif

// source: http://blogs.aerys.in/jeanmarc-leroux/2012/05/25/optimized-aabb-ray-intersection/
bool RayTracer::RayBoxCollide(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax, const glm::mat4& m)
{
	glm::mat4 invTranslation = glm::inverse(glm::mat4(
		1,	0,	0,	m[0][3],
		0,	1,	0,	m[1][3],
		0,	0,	1,	m[2][3],
		0,	0,	0,	1
	));
	glm::mat4 invRotation = glm::inverse(glm::mat4(
		m[0][0],	m[0][1],	m[0][2],	0,
		m[1][0],	m[1][1],	m[1][2],	0,
		m[2][0],	m[2][1],	m[2][2],	0,
		0,			0,			0,			1
	));
	Ray	localRay(invRotation*invTranslation*ray.position, invRotation*ray.direction);
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