#pragma once
#ifndef FLOOR_H
#define FLOOR_H

#define FLOOR_VERTICES {\
-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,\
0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,\
0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,\
-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}

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
	Instance<RectangularPrism>* CreateInstance() override
	{
		Instance<RectangularPrism>* instance 
			= RectangularPrism::CreateInstance();

		m_oBodyDef.bodyType = q3BodyType::eStaticBody;
		q3Transform localSpace;
		q3Identity(localSpace);
		mBoxDef.Set(localSpace, q3Vec3(instance->mSize.x, instance->mSize.y, 0.0));	// set the size, ignore thickness
		instance->mBody = q3scene.CreateBody(m_oBodyDef);
		mBox = instance->mBody->AddBox(mBoxDef);
		instance->mReady = true;

		return instance;
	}
	void MoveInstance(Instance<RectangularPrism>* instance, q3Vec3 translate)
	{
		const q3Transform prev = instance->mBody->GetTransform();
		q3Transform updated = q3Transform(prev.position+translate, prev.rotation);
		instance->mBody->SetTransform(updated.position, updated.rotation);
	}

private:
	void OpenGLDraw()
	{
		m_ovFaces[0]->Draw();
	}
};

#endif // FLOOR_H