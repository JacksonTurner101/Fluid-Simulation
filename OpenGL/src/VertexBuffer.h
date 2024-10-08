#pragma once
#include <glew.h>

class VertexBuffer {

	unsigned static int boundVB;

private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();

	void AddData(const void* data, unsigned int size);
	void Bind() const;
	void Unbind() const;
	void ReserveSpace(unsigned int size, GLenum usage) const;
};