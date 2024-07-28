#include "Square.h"

void Square::Draw()
{
	m_Shader.Bind();
	m_Vao.Bind();
	m_Ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, 8, GL_UNSIGNED_INT, nullptr));
}

Square::Square()
{
	//setting some default values for a square shape
	//top left
	m_Vertices[0] = -0.5f;
	m_Vertices[1] = 0.5f;
	//top right
	m_Vertices[2] = 0.5f;
	m_Vertices[3] = 0.5f;
	//bottom left
	m_Vertices[4] = -0.5f;
	m_Vertices[5] = -0.5f;
	//bottom right
	m_Vertices[6] = 0.5f;
	m_Vertices[7] = -0.5f;

	m_Indices[0] = 0;
	m_Indices[1] = 3;
	m_Indices[2] = 1;
	m_Indices[3] = 0;
	m_Indices[4] = 2;
	m_Indices[5] = 3;

	m_Vb.AddData(m_Vertices, 8 * sizeof(float));
	m_Ib.AddData(m_Indices, 6);
	m_Vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}
