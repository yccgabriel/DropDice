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
	}
	~Die()
	{
		for (int i = 0; i < 6; i++)
		{
			delete m_ovFaces[i];
		}
	}
	void DrawDie()
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