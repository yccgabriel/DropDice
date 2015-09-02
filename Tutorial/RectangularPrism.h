#pragma once
#ifndef RECTANGULARPRISM_H
#define RECTANGULARPRISM_H

#define VERTICES_COLUMN 8

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "ShaderManager.h"

struct Instance;	// forward definition
class RectangularPrism
{
public:
	std::vector<float> m_fvVertices;
	std::vector<Instance*> m_opvInstances;
	GLuint m_vbo;	// vertex buffer object
	ShaderManager*	m_opShaderManager;

	RectangularPrism(const std::vector<float>& v);
	~RectangularPrism();
	void Draw();
	void DrawInstance(Instance*);
	void CreateInstance();
private:
	void RectangleToTriangleVertices();
};

struct Instance	// default all to be public
{
	glm::mat4 transform;
};

#endif // RECTANGULARPRISM_H