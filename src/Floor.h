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
	Instance* CreateInstance() override
	{
		Instance* instance 
			= RectangularPrism::CreateInstance();

		m_oBodyDef.bodyType = q3BodyType::eStaticBody;
		q3Transform localSpace;
		q3Identity(localSpace);
		mBoxDef.Set(localSpace, q3Vec3(instance->mSize.x, instance->mSize.y, 0.0));	// set the size, ignore thickness
		instance->mBody = q3scene.CreateBody(m_oBodyDef);
		mBox = instance->mBody->AddBox(mBoxDef);

		instance->mBoxMinXYZ = glm::vec3(-0.5, -0.5, -0.001);
		instance->mBoxMaxXYZ = glm::vec3(0.5, 0.5, 0.001);

		instance->mReady = true;
		return instance;
	}
	static void TranslateInstance(Instance* instance, glm::vec3 f, glm::vec3 t)
	{
		TranslateInstance(instance, q3Vec3(f.x, f.y, f.z), q3Vec3(t.x, t.y, t.z));
	}
	static void TranslateInstance(Instance* instance, q3Vec3 from, q3Vec3 translate)		// ray-tracing calculate the translate-vector
	{
		const q3Transform prev = instance->mBody->GetTransform();
		q3Transform updated;
		updated.position = from + translate;
		updated.rotation = prev.rotation;
		instance->mBody->SetTransform(updated.position, updated.rotation);		// TODO: this function does not exists in qu3e. redo the physics engine
	}
	void RotateInstance() {}

private:
	void OpenGLDraw()
	{
		m_ovFaces[0]->Draw();
	}
};

#endif // FLOOR_H