#pragma once
#ifndef ROD_H
#define ROD_H

#define ROD_VERTICES {\
-1.0f, -0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, \
1.0f, -0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  \
1.0f, 0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   \
-1.0f, 0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  \
												   \
-1.0f, -0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  \
1.0f, -0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   \
1.0f, 0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	   \
-1.0f, 0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,   \
												   \
-1.0f, 0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   \
-1.0f, 0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  \
-1.0f, -0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, \
-1.0f, -0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  \
												   \
1.0f, 0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	   \
1.0f, 0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   \
1.0f, -0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  \
1.0f, -0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,   \
												   \
-1.0f, -0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, \
1.0f, -0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  \
1.0f, -0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,   \
-1.0f, -0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  \
												   \
-1.0f, 0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  \
1.0f, 0.01f, -0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,   \
1.0f, 0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	   \
-1.0f, 0.01f, 0.01f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f    }

#include "RectangularPrism.h"

extern q3Scene q3scene;

class Rod : public RectangularPrism
{
public:
	Rod() : RectangularPrism(ROD_VERTICES)
	{
		// map Faces vertices pointer to m_fvVertices
		for (int i = 0; i < 6; i++)
		{
			m_ovFaces.push_back(new Face());
			m_ovFaces[i]->m_fpVertices = &m_fvVertices[i*VERTICES_COLUMN * 6];
			m_ovFaces[i]->texture = NULL;
			m_opShaderManager->DefineAttribs();		// why need this?
		}
	}
	~Rod()
	{
		for (int i = 0; i < 6; i++)
		{
			delete m_ovFaces[i];
		}
	}
	Instance* CreateInstance() override
	{
		Instance* instance = RectangularPrism::CreateInstance();
		
		instance->mBoxMinXYZ = glm::vec3(-1, -0.01, -0.01);
		instance->mBoxMaxXYZ = glm::vec3(1, 0.01, 0.01);

		instance->mReady = true;
		return instance;
	}
	void RotateInstanceInLocalSpace(Instance* instance, const glm::mat4& rotmat)
	{
		glm::vec4 temp;
		instance->localtrans = rotmat;
		//temp = rotmat*glm::vec4(instance->mBoxMinXYZ, 1);
		//instance->mBoxMinXYZ = -glm::vec3(std::abs(temp.x), std::abs(temp.y), std::abs(temp.z));
		//temp = rotmat*glm::vec4(instance->mBoxMaxXYZ, 1);
		//instance->mBoxMaxXYZ = glm::vec3(std::abs(temp.x), std::abs(temp.y), std::abs(temp.z));
		std::cout << "mBoxMinXYZ: " << instance->mBoxMinXYZ.x << ", " << instance->mBoxMinXYZ.y << ", " << instance->mBoxMinXYZ.z << std::endl;
		std::cout << "mBoxMaxXYZ: " << instance->mBoxMaxXYZ.x << ", " << instance->mBoxMaxXYZ.y << ", " << instance->mBoxMaxXYZ.z << std::endl;
	}
	void DrawInstance(Instance* instance) override		// currently not called by any function
	{
		if (!instance->mReady)	return;
		glUniformMatrix4fv(m_opShaderManager->resources.uniforms.model, 1, GL_FALSE, glm::value_ptr(instance->transform));
		this->OpenGLDraw();
	}
	void DrawInstance(Instance* instance, const glm::vec3& color) // draw with color. This function not exist in base class
	{
		if (!instance->mReady)	return;
		glUniformMatrix4fv(m_opShaderManager->resources.uniforms.model, 1, GL_FALSE, glm::value_ptr(instance->transform));
		glUniform3f(m_opShaderManager->resources.uniforms.overrideColor, color.r, color.g, color.b);
		glUniform1i(m_opShaderManager->resources.uniforms.useTexture, false);
		this->OpenGLDraw();
		glUniform3f(m_opShaderManager->resources.uniforms.overrideColor, 1.0f, 1.0f, 1.0f);
		glUniform1i(m_opShaderManager->resources.uniforms.useTexture, true);
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

#endif // ROD_H