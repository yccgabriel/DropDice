#pragma once
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <glm/glm.hpp>

namespace RayTracer
{
	struct Ray
	{
		glm::vec4 position;		glm::vec4& p = position;
		glm::vec4 direction;	glm::vec4& d = direction;
		Ray() {}
		Ray(const glm::vec4& pos, const glm::vec4& dir) { position = pos, direction = dir; }
		Ray(const glm::vec3& pos, const glm::vec3& dir) { position = glm::vec4(pos, 1), direction = glm::vec4(dir, 1); }
		//Ray operator*(const glm::mat4& mat) { return Ray(mat*this->position, mat*this->direction); }		// don't know why not work
		friend Ray operator*(const glm::mat4& mat, const Ray& ray) { return Ray(mat*ray.position, mat*ray.direction); }
#ifdef _DEBUG
		friend std::ostream& operator<<(std::ostream& out, const Ray& l)
		{
			out << l.p.x << ", " << l.p.y << ", " << l.p.z << "//  " << l.d.x << ", " << l.d.y << ", " << l.d.z;
			return out;
		}
#endif
	};
	struct Line		// store vector in vec3
	{
		glm::vec3 position;		glm::vec3& p = position;
		glm::vec3 direction;	glm::vec3& d = direction;
		//float k;	// L = p + kd	in vector form. k as parameter
		Line() {}
		Line(const Ray& ray) { this->position = glm::vec3(ray.position); this->direction = glm::vec3(ray.direction); }
		Line(const glm::vec4& pos, const glm::vec4& dir) { this->position = glm::vec3(pos), this->direction = glm::vec3(dir); }
		Line(const glm::vec3& pos, const glm::vec3& dir) { this->position = pos; this->direction = dir; }
		friend Line operator*(const glm::mat4& m, const Line& l) { return Line(m*glm::vec4(l.p, 1), m*glm::vec4(l.d, 1)); }
		Line& operator=(const Line& rhs) { p = rhs.p; d = rhs.d; return *this; }
		glm::vec3 GetPoint(float k) const { return position + k*direction; }
#ifdef _DEBUG
		friend std::ostream& operator<<(std::ostream& out, const Line& l)
		{
			out << l.p.x << ", " << l.p.y << ", " << l.p.z << "//  " << l.d.x << ", " << l.d.y << ", " << l.d.z;
			return out;
		}
#endif
	};
	bool RayBoxCollide(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax, const glm::mat4& model);
	glm::vec3 SkewLinesNearestPoint(const Ray&, const Ray&);	// return the nearest point on the first line(1st parameter)
	glm::vec3 SkewLinesNearestPoint(const Line&, const Line&);
	glm::vec3 SkewLinesNearestPoint(const Line&, const Line&, glm::vec3&);
}
namespace RT = RayTracer;

#endif // RAYTRACER_H