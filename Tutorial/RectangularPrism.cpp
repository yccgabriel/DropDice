#ifdef _DEBUG
	#include <iostream>
#endif
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
	for (int i = 0; i < m_opvInstances.size(); i++)
		delete m_opvInstances[i];
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