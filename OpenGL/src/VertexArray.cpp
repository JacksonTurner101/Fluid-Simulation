#include "VertexArray.h"
#include "Renderer.h"
#include <iostream>

//static variable to keep track of currently bound vao

unsigned int VertexArray::boundVAO = 0;

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_Id));
}

VertexArray::~VertexArray() {

}

void VertexArray::AddVertexAttribPointer(int size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
	Bind();
	GLCall(glEnableVertexAttribArray(m_CurrentIndex));
	GLCall(glVertexAttribPointer(m_CurrentIndex, size, type, normalized, stride, pointer));
	m_CurrentIndex++;
}

void VertexArray::Bind() const
{
	if (boundVAO != m_Id) {
		GLCall(glBindVertexArray(m_Id));
		boundVAO = m_Id;
	}
}

void VertexArray::Unbind()
{
	GLCall(glBindVertexArray(0));
	boundVAO = 0;
}

int VertexArray::QueryCurrentVAO()
{
	return boundVAO;
}



