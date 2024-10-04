#include "Renderer.h"
#include "VertexBuffer.h"

unsigned int VertexBuffer::boundVB = 0;

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer()
{
    GLCall(glGenBuffers(1, &m_RendererID));
}

void VertexBuffer::AddData(const void* data, unsigned int size) {
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1,&m_RendererID));
}

void VertexBuffer::Bind() const
{
    if (boundVB != m_RendererID) {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
        boundVB = m_RendererID;
    }

}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

}

void VertexBuffer::ReserveSpace(unsigned int size, GLenum usage) const
{
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage));
}
