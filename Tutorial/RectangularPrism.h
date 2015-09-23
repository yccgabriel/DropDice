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
#include "ShaderManager.h"

extern q3Scene q3scene;
struct Instance;	// forward definition
struct Face;		// forward definition
class RectangularPrism
{
public:
	q3BodyDef	m_oBodyDef;
	q3BoxDef	mBoxDef;
	const q3Box* mBox;
	std::vector<float>	m_fvVertices;
	std::vector<Face*>	m_ovFaces;
	std::deque<Instance*> mInstances;
	GLuint m_vbo;	// vertex buffer object
	ShaderManager* m_opShaderManager;

	RectangularPrism(const std::vector<float>& v);
	~RectangularPrism();
	void CreateInstance();
	virtual void DeleteInstance(Instance*);		// this function called by DeleAllInstances()
	void DeleteAllInstances();
	void DrawInstance(Instance*);
private:
	void RectangleToTriangleVertices();
	virtual void OpenGLDraw() = 0;
};

struct Instance	// default all to be public
{
	glm::mat4 transform;
	q3Body* mBody;
	bool mReady;		// flag indicate instance is completely created to draw

	Instance() { mReady = false; }
	~Instance(){}
	void SetTransform()		// convert engine's transform to OpenGL's transform
	{
		const q3Transform& q3tran = mBody->GetTransform();
		const q3Vec3& t = q3tran.position;
		const q3Mat3& r = q3tran.rotation;
		glm::mat4	rot(r.ex.x, r.ex.y, r.ex.z, 0,
						r.ey.x, r.ey.y, r.ey.z, 0,
						r.ez.x, r.ez.y, r.ez.z, 0,
							 0,		 0,		 0, 1);
		glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(t.x, t.y, t.z));
		transform = trans * rot;

		// debugging
		//std::cout << r.ex.x << "\t" << r.ex.y << "\t" << r.ex.z << std::endl;
		//std::cout << r.ey.x << "\t" << r.ey.y << "\t" << r.ey.z << std::endl;
		//std::cout << r.ez.x << "\t" << r.ez.y << "\t" << r.ez.z << std::endl;
		//std::cout << std::endl;
	}
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