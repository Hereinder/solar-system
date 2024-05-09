#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexBuffer {
public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer() = default;

    void Bind() const;
    void Unbinde() const;
    void Update(const void *data, unsigned int size) const;
    void SubUpdate(const void *data, unsigned int size, unsigned int offset) const;

private:
    unsigned int m_Renderer_Id;
};