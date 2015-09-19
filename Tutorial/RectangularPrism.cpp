#ifdef _DEBUG
	#include <iostream>
	#include <sstream>
	#include <Windows.h>
#endif
#include <algorithm>	// for std::remove
#include "RectangularPrism.h"

RectangularPrism::RectangularPrism(const std::vector<float>& v) : m_fvVertices(v)
{
	m_opShaderManager = ShaderManager::GetInstance();
	RectangleToTriangleVertices();
	m_opShaderManager->DefineAttribs();	// this call must follows Bind Buffer
}

RectangularPrism::~RectangularPrism()
{
	glDeleteBuffers(1, &m_vbo);
	for (int i = 0; i < mInstances.size(); i++)
		delete mInstances[i];
}

void RectangularPrism::CreateInstance()
{
	mInstances.push_back(new Instance());
}

void RectangularPrism::DeleteInstance(Instance* instance)
{
	dynamicsWorld->removeRigidBody(instance->mRigidBody);
	mInstances.erase(std::remove(mInstances.begin(), mInstances.end(), instance), mInstances.end());
	delete instance;
}

void RectangularPrism::DeleteAllInstances()
{
	for (int i = mInstances.size() - 1; i >= 0; i--)
	{
		DeleteInstance(mInstances[i]);
	}
//	for (std::deque<Instance*>::reverse_iterator it = mInstances.rbegin(); it != mInstances.rend(); ++it)
//		DeleteInstance(*it);
}

void RectangularPrism::DrawInstance(Instance* instance)
{
	instance->SetTransform();
	glUniformMatrix4fv(m_opShaderManager->resources.uniforms.model, 1, GL_FALSE, glm::value_ptr(instance->transform));
	this->OpenGLDraw();
}

// private methods
void RectangularPrism::RectangleToTriangleVertices()
{
	// assume rectangle vertices arrange in anti-clockwise order for each face
	if (m_fvVertices.size() % (VERTICES_COLUMN * 4) != 0)
	{
#ifdef _DEBUG
		std::cout << "Check Vertices definition" << std::endl;
#endif
		return;
	}
	int gp = (m_fvVertices.size()-1) / (VERTICES_COLUMN * 4) ;
	int oldSize = m_fvVertices.size() / VERTICES_COLUMN;
	m_fvVertices.resize(m_fvVertices.size() / 4 * 6);
	int newSize = m_fvVertices.size() / VERTICES_COLUMN;
	for (int i = oldSize - 1; i >= 0; i--)
	{
		//std::cout << i << std::endl;
		switch (i % 4){
		case 3:
			for (int j = VERTICES_COLUMN - 1; j >= 0; j--)
				m_fvVertices[(gp * 6 + 4)*VERTICES_COLUMN + j]													= m_fvVertices[(gp * 4 + 3)*VERTICES_COLUMN + j];
			break;
		case 2:
			for (int j = VERTICES_COLUMN - 1; j >= 0; j--)
				m_fvVertices[(gp * 6 + 2)*VERTICES_COLUMN + j] = m_fvVertices[(gp * 6 + 3)*VERTICES_COLUMN + j] = m_fvVertices[(gp * 4 + 2)*VERTICES_COLUMN + j];
			break;
		case 1:
			for (int j = VERTICES_COLUMN - 1; j >= 0; j--)
				m_fvVertices[(gp * 6 + 1)*VERTICES_COLUMN + j]													= m_fvVertices[(gp * 4 + 1)*VERTICES_COLUMN + j];
			break;
		case 0:
			for (int j = VERTICES_COLUMN - 1; j >= 0; j--)
				m_fvVertices[(gp * 6 + 0)*VERTICES_COLUMN + j] = m_fvVertices[(gp * 6 + 5)*VERTICES_COLUMN + j] = m_fvVertices[(gp * 4 + 0)*VERTICES_COLUMN + j];
			gp--;
			break;
		default:
			break;
		}
	}
		
}


Face::Face()
{
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	m_opShaderManager = ShaderManager::GetInstance();
}

void Face::Draw()
{
	m_opShaderManager->UseTexture(texture);
	glBufferData(GL_ARRAY_BUFFER, VERTICES_COLUMN*6*sizeof(float), m_fpVertices, GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES_COLUMN*6);
}