#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_Id));
}

VertexArray::~VertexArray() {

}

void VertexArray::AddVertexAttribPointer(int size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
	GLCall(glBindVertexArray(m_Id));
	GLCall(glEnableVertexAttribArray(m_CurrentIndex));
	GLCall(glVertexAttribPointer(m_CurrentIndex, size, type, normalized, stride, pointer));
	m_CurrentIndex++;
}

void VertexArray::Bind()
{
	GLCall(glBindVertexArray(m_Id));
}

void VertexArray::Unbind()
{
	GLCall(glBindVertexArray(0));
}

