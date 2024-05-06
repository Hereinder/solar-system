#include "IndexBuffer.hpp"

#include "glTools.hpp"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : m_Count(count) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_Renderer_Id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_Id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_Id)); }

void IndexBuffer::Unbinde() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
