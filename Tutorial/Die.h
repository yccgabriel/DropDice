#pragma once
#ifndef DIE_H
#define DIE_H

#define DIE_VERTICES {\
-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, \
0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  \
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   \
-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  \
												   \
-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  \
0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   \
0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	   \
-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,   \
												   \
-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   \
-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  \
-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, \
-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  \
												   \
0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	   \
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   \
0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  \
0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   \
												   \
-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, \
0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  \
0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   \
-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  \
												   \
-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  \
0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   \
0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	   \
-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f    }

#include "RectangularPrism.h"

extern btDiscreteDynamicsWorld* dynamicsWorld;
extern q3Scene scene;

class Die : public RectangularPrism
{
public:
	q3BodyDef	m_oBodyDef;
	q3Body*		m_opBody;
	q3BoxDef	m_oBoxDef;
	q3Transform	m_oLocalSpace;
	Die() : RectangularPrism(DIE_VERTICES)
	{
		m_opBody = scene.CreateBody(m_oBodyDef);
		q3Identity(m_oLocalSpace);
		m_oBoxDef.Set(m_oLocalSpace, q3Vec3(1.0, 1.0, 1.0));
		// map Faces vertices pointer to m_fvVertices
		for (int i = 0; i < 6; i++)
		{
			m_ovFaces.push_back(new Face());
			m_ovFaces[i]->m_fpVertices = &m_fvVertices[i*VERTICES_COLUMN * 6];
			m_ovFaces[i]->texture = m_opShaderManager->resources.textures[i];
		}

		m_CollisionShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
	}
	~Die()
	{
		for (int i = 0; i < 6; i++)
		{
			delete m_ovFaces[i];
		}
		delete m_CollisionShape;
	}
	void CreateInstance()
	{
		// random generator
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist(0, 1);
		// end of random generator
		RectangularPrism::CreateInstance();
		Instance* instance = mInstances.back();	// the instance just created
		instance->mMotionState = new btDefaultMotionState(btTransform(btQuaternion(dist(mt), dist(mt), dist(mt), 1), btVector3(0, 0, 50)));
		// calculate inertia
		btScalar mass = 100;
		btVector3 dieInertia(0, 0, 0);
		m_CollisionShape->calculateLocalInertia(mass, dieInertia);	// mass and inertia vector
		btRigidBody::btRigidBodyConstructionInfo dieRigidBodyCI(mass, instance->mMotionState, m_CollisionShape, dieInertia);
		instance->mRigidBody = new btRigidBody(dieRigidBodyCI);
		dynamicsWorld->addRigidBody(instance->mRigidBody);
	}

private:
	void OpenGLDraw() override
	{
		for (int i = 0; i < m_ovFaces.size(); i++)
		{
			m_ovFaces[i]->Draw();
		}
	}
	void CreateInstance()
	{
		RectangularPrism::CreateInstance();
		m_opvInstances.back()->m_opBox = m_opBody->AddBox(m_oBoxDef);
	}
};

#endif // DIE_H