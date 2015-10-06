#pragma once
#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <glm/glm.hpp>

namespace RayTracer
{
	struct Ray
	{
		glm::vec4 position;
		glm::vec4 direction;
		Ray() {}
		Ray(const glm::vec4& pos, const glm::vec4& dir) { position = pos, direction = dir; }
		Ray(const glm::vec3& pos, const glm::vec3& dir) { position = glm::vec4(pos, 1), direction = glm::vec4(dir, 1); }
		//Ray operator*(const glm::mat4& mat) { return Ray(mat*this->position, mat*this->direction); }		// don't know why not work
		friend Ray operator*(const glm::mat4& mat, const Ray& ray) { return Ray(mat*ray.position, mat*ray.direction); }
	};
	bool RayBoxCollide(const Ray& ray, const glm::vec3& boxMin, const glm::vec3& boxMax, const glm::mat4& model);
}

#endif // RAYTRACER_H