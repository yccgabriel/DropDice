#pragma once
#ifndef RECTANGULARPRISM_H
#define RECTANGULARPRISM_H

#define VERTICES_COLUMN 8

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "ShaderManager.h"
#include "ShaderManager.h"

struct Instance;	// forward definition
struct Face;		// forward definition
class RectangularPrism
{
public:
	std::vector<float>	m_fvVertices;
	std::vector<Face*>	m_ovFaces;
	std::vector<Instance*> m_opvInstances;
	GLuint m_vbo;	// vertex buffer object
	ShaderManager* m_opShaderManager;

	RectangularPrism(const std::vector<float>& v);
	~RectangularPrism();
	void CreateInstance();
private:
	void RectangleToTriangleVertices();
};

struct Instance	// default all to be public
{
	glm::mat4 transform;
};

struct Face
{
	float* m_fpVertices;
	GLuint m_vbo;
	GLuint texture;
	ShaderManager* m_opShaderManager;

	Face();
	void Draw();
};

#endif // RECTANGULARPRISM_H