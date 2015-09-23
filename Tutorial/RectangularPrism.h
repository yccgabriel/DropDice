#pragma once
#ifndef RECTANGULARPRISM_H
#define RECTANGULARPRISM_H
#ifdef _DEBUG
#include <sstream>
#endif

#define VERTICES_COLUMN 8

#include <vector>
#include <deque>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <qu3e/q3.h>

#include "ShaderManager.h"
#include "Instance.h"

extern q3Scene q3scene;
struct Face;		// forward definition
class RectangularPrism
{
public:
	q3BodyDef	m_oBodyDef;
	q3BoxDef	mBoxDef;
	const q3Box* mBox;
	std::vector<float>	m_fvVertices;
	std::vector<Face*>	m_ovFaces;
	std::deque< Instance<RectangularPrism>* > mInstances;
	GLuint m_vbo;	// vertex buffer object
	ShaderManager* m_opShaderManager;

	RectangularPrism(const std::vector<float>& v);
	~RectangularPrism();
	virtual Instance<RectangularPrism>* CreateInstance();
	virtual void DeleteInstance( Instance<RectangularPrism>* );		// this function called by DeleAllInstances()
	void DeleteAllInstances();
	void DrawInstance( Instance<RectangularPrism>* );
private:
	void RectangleToTriangleVertices();
	virtual void OpenGLDraw() = 0;
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