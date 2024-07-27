#include "Shape.h"

#include <glew.h>

IndexBuffer Shape::GetIndexBuffer() {

	return m_Ib;
}

VertexArray Shape::GetVertexArray()
{
	return m_Vao;
}

Shader Shape::GetShader()
{
	return m_Shader;
}

Shape::Shape() {
	
}
