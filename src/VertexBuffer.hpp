#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexBuffer {
public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer() = default;

    void Bind() const;
    void Unbinde() const;

private:
    unsigned int m_Renderer_Id;
};