#pragma once
#ifndef FLOOR_H
#define FLOOR_H

#define FLOOR_VERTICES {\
-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,\
1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,\
1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,\
-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}

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

		m_oBodyDef.bodyType = q3BodyType::eStaticBody;
		q3Transform localSpace;
		q3Identity(localSpace);
		mBoxDef.Set(localSpace, q3Vec3(1.0, 1.0, 0.0));	// x,y,z
	}
	~Floor()
	{
		delete m_ovFaces[0];
	}
	void CreateInstance() 
	{
		RectangularPrism::CreateInstance();
		Instance* instance = mInstances.back();	// the instance just created

		instance->mBody = q3scene.CreateBody(m_oBodyDef);
		mBox = instance->mBody->AddBox(mBoxDef);
		instance->mReady = true;
	}

private:
	void OpenGLDraw()
	{
		m_ovFaces[0]->Draw();
	}
};

#endif // FLOOR_H