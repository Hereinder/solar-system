#pragma once
#include <GL/glew.h>

#include <vector>

#include "glTools.hpp"
struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
        case GL_FLOAT:
            return 4;
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        }
        ASSERT(false)
        return 0;
    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout() : m_Stride(0){};
    ~VertexBufferLayout() = default;

    template <typename T>
    void Push(unsigned int count) {
        static_assert(false);
    }

    inline unsigned int GetStride() const { return m_Stride; }
    inline const std::vector<VertexBufferElement> &GetElements() const { return m_Elements; }

private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};

template <>
void VertexBufferLayout::Push<float>(unsigned int count);
template <>
void VertexBufferLayout::Push<unsigned int>(unsigned int count);
template <>
void VertexBufferLayout::Push<unsigned char>(unsigned int count);
