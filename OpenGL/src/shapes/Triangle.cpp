#include "Triangle.h"

void Triangle::Draw()
{
	m_Shader.Bind();
	m_Vao.Bind();
	m_Ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_INT, nullptr));

}

Triangle::Triangle() {
	//setting some default values for a triangle shape
	m_Vertices[0] = 0.0f;
	m_Vertices[1] = 0.5f;
	m_Vertices[2] = -0.5f;
	m_Vertices[3] = 0.0f;
	m_Vertices[4] = 0.5f;
	m_Vertices[5] = 0.0f;
	m_Indices[0] = 0;
	m_Indices[1] = 1;
	m_Indices[2] = 2;

	m_Vb.AddData(m_Vertices, 6 * sizeof(float));
	m_Ib.AddData(m_Indices, 3);
	m_Vao.AddVertexAttribPointer(2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}