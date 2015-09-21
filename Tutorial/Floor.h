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
	void CreateInstance() 
	{
		RectangularPrism::CreateInstance();
		Instance* instance = mInstances.back();	// the instance just created
	}

private:
	void OpenGLDraw()
	{
		m_ovFaces[0]->Draw();
	}
};

#endif // FLOOR_H