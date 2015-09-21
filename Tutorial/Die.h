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

extern q3Scene scene;

class Die : public RectangularPrism
{
public:
	q3BodyDef	m_oBodyDef;
	q3BoxDef	mBoxDef;
	const q3Box* mBox;
	Die() : RectangularPrism(DIE_VERTICES)
	{
		// map Faces vertices pointer to m_fvVertices
		for (int i = 0; i < 6; i++)
		{
			m_ovFaces.push_back(new Face());
			m_ovFaces[i]->m_fpVertices = &m_fvVertices[i*VERTICES_COLUMN * 6];
			m_ovFaces[i]->texture = m_opShaderManager->resources.textures[i];
		}
		q3Transform localSpace;
		q3Identity(localSpace);
		mBoxDef.Set(localSpace, q3Vec3(1.0, 1.0, 1.0));	// width, height, depth
	}
	~Die()
	{
		for (int i = 0; i < 6; i++)
		{
			delete m_ovFaces[i];
		}
	}
	void CreateInstance()
	{
		// random generator
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist(-1, 1);
		// end of random generator		how to use: dist(mt);
		RectangularPrism::CreateInstance();
		Instance* instance = mInstances.back();	// the instance just created

		instance->mBody = scene.CreateBody(m_oBodyDef);
		mBox = instance->mBody->AddBox(mBoxDef);
		instance->mReady = true;
	}
	void DeleteInstance(Instance* instance) override
	{
		instance->mBody->RemoveBox(mBox);
		scene.RemoveBody(instance->mBody);
		RectangularPrism::DeleteInstance(instance);
	}

private:
	void OpenGLDraw() override
	{
		for (int i = 0; i < m_ovFaces.size(); i++)
		{
			m_ovFaces[i]->Draw();
		}
	}
};

#endif // DIE_H