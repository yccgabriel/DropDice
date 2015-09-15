#pragma once
#ifndef FLOOR_H
#define FLOOR_H

#define FLOOR_VERTICES {\
-2.0f, -2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,\
2.0f, -2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,\
2.0f, 2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,\
-2.0f, 2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}

#include "RectangularPrism.h"

extern btDiscreteDynamicsWorld* dynamicsWorld;

class Floor : public RectangularPrism
{
public:
	Floor() : RectangularPrism(FLOOR_VERTICES)
	{ 
		m_ovFaces.push_back(new Face());
		m_ovFaces[0]->m_fpVertices = &m_fvVertices[0];
		m_ovFaces[0]->texture = NULL;
		m_opShaderManager->DefineAttribs();		// why need this?

		m_CollisionShape = new btBoxShape(btVector3(1,1,0));
		//m_CollisionShape = new btStaticPlaneShape(btVector3(0, 0, 1), 1);
	}
	~Floor()
	{
		delete m_ovFaces[0];
	}
	void DrawFloor()
	{
		m_ovFaces[0]->Draw();
	}
	void CreateInstance() 
	{
		RectangularPrism::CreateInstance();
		Instance* instance = m_opvInstances.back();	// the instance just created
		instance->mMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
		btRigidBody::btRigidBodyConstructionInfo floorRigidBodyCI(0, instance->mMotionState, m_CollisionShape);
		instance->mRigidBody = new btRigidBody(floorRigidBodyCI);
		dynamicsWorld->addRigidBody(instance->mRigidBody);
	}
};

#endif // FLOOR_H