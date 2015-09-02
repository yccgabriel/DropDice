#ifdef _DEBUG
	#include <iostream>
#endif
#include "ShaderUtil.h"
#include "RectangularPrism.h"

RectangularPrism::RectangularPrism(const std::vector<float>& v) : m_fvVertices(v)
{
	RectangleToTriangleVertices();
	glGenBuffers(1, &m_vbo);
	// Make the buffer the active array buffer
	// i.e. opengl->array_buffer = buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);	
	
	//m_opShaderManager = ShaderManager::GetInstance();
}

RectangularPrism::~RectangularPrism()
{
	glDeleteBuffers(1, &m_vbo);
	for (int i = 0; i < m_opvInstances.size(); i++)
		delete m_opvInstances[i];
}

void RectangularPrism::Draw()
{
	glBufferData(GL_ARRAY_BUFFER, m_fvVertices.size()*sizeof(float), &m_fvVertices[0], GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, m_fvVertices.size() / VERTICES_COLUMN);
}
void RectangularPrism::DrawInstance(Instance* instance)
{
//	glm::mat4 model;
//	model = instance->transform;
//	glUniformMatrix4fv(m_opShaderManager->resources.uniforms.model, 1, GL_FALSE, glm::value_ptr(model));
//
	// Upload a bunch of data into the active array buffer
	// i.e. opengl->array_buffer = new buffer(sizeof(points), points, STATIC_DRAW)
	glBufferData(GL_ARRAY_BUFFER, m_fvVertices.size()*sizeof(float), &m_fvVertices[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, m_fvVertices.size()/VERTICES_COLUMN);
}

void RectangularPrism::CreateInstance()
{
	m_opvInstances.push_back(new Instance());
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

	// testing
//	for (int i = 0; i < 6; i++)
//	{
//		for (int j = 0; j < VERTICES_COLUMN; j++)
//			std::cout << m_fvVertices[8*i+j] << "\t";
//		std::cout << std::endl;
//	}
		
}