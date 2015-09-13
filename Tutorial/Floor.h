#pragma once
#ifndef FLOOR_H
#define FLOOR_H

#define FLOOR_VERTICES {\
-2.0f, -2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,\
2.0f, -2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,\
2.0f, 2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,\
-2.0f, 2.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}

#include "RectangularPrism.h"

class Floor : public RectangularPrism
{
public:
	// Initial position and orientation of the rigid body 
	rp3d::Vector3		m_InitPosition = rp3d::Vector3(0.0, 0.0, 0.0);
	rp3d::Quaternion	m_InitOrientation = rp3d::Quaternion::identity();
	rp3d::Transform		m_Transform = rp3d::Transform(m_InitPosition, m_InitOrientation);

	Floor() : RectangularPrism(FLOOR_VERTICES)
	{ 
		m_ovFaces.push_back(new Face());
		m_ovFaces[0]->m_fpVertices = &m_fvVertices[0];
		m_ovFaces[0]->texture = NULL;
		m_opShaderManager->DefineAttribs();		// why need this?
	}
	~Floor()
	{
		delete m_ovFaces[0];
	}
	void DrawFloor()
	{
		m_ovFaces[0]->Draw();
	}
	void CreateInstance() override
	{
		m_opvInstances.push_back(new Instance(FLOOR, m_Transform));
	}
};

#endif // FLOOR_H