#pragma once
#ifndef RECTANGULARPRISM_H
#define RECTANGULARPRISM_H

#define VERTICES_COLUMN 8

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <rp3d/reactphysics3d.h>

#include "ShaderManager.h"
#include "ShaderManager.h"

extern rp3d::DynamicsWorld world;

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
	virtual void CreateInstance() = 0;	// pure virtual function
private:
	void RectangleToTriangleVertices();
};

#define DIE			0
#define FLOOR		1
struct Instance
{
	rp3d::RigidBody* m_pBody;
	rp3d::ProxyShape* m_pProxyShape;
	glm::mat4 transform;

	Instance(int instanceType, rp3d::Transform transform)
	{
		rp3d::Transform t = rp3d::Transform::identity();
		if (instanceType == DIE)
		{
			rp3d::Vector3 halfExtents(0.5, 0.5, 0.5);
			rp3d::BoxShape boxShape(halfExtents);
			m_pBody = world.createRigidBody(transform);
			m_pBody->setType(rp3d::DYNAMIC);
			std::cout << "Die body address \t\t"<<m_pBody << std::endl;
			rp3d::decimal mass = rp3d::decimal(4.0);
			m_pProxyShape = m_pBody->addCollisionShape(boxShape, t, mass);
		}
		else if (instanceType == FLOOR)
		{
			rp3d::Vector3 halfExtents(1.0, 1.0, 0.5);
			rp3d::BoxShape boxShape(halfExtents);
			m_pBody = world.createRigidBody(transform);
			std::cout << "Floor body address \t\t" << m_pBody << std::endl;
			m_pBody->setType(rp3d::STATIC);
			//rp3d::decimal mass = rp3d::decimal(100.0);
			m_pProxyShape = ((rp3d::CollisionBody*)m_pBody)->addCollisionShape(boxShape, t);
		}
	}
	void Update()
	{
		rp3d::decimal matrix[16];
		m_pBody->getTransform().getOpenGLMatrix(matrix);
		transform = glm::make_mat4(matrix);
		std::cout << m_pBody->getTransform().getPosition().z << std::endl;
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