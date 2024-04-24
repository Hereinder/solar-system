#include "VertexBuffer.hpp"

#include "glTools.hpp"

VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
    GLCall(glGenBuffers(1, &m_Renderer_Id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_Id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Renderer_Id)); }

void VertexBuffer::Unbinde() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
