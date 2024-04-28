#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IndexBuffer {
public:
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer() = default;

    void Bind() const;
    void Unbinde() const;

    inline unsigned int GetCount() const { return m_Count; }

private:
    unsigned int m_Renderer_Id;
    unsigned int m_Count;
};