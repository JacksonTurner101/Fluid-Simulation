#pragma once

#include "VertexBuffer.h"
#include <glew.h>

class VertexArray {

private:
	unsigned int m_Id;
	unsigned int m_CurrentIndex = 0;
public:
	VertexArray();
	~VertexArray();
	//void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void AddVertexAttribPointer(int size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	void Bind() const;
	void Unbind();
};