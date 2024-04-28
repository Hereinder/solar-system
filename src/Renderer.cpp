#include "Renderer.hpp"

#include <GL/glew.h>

Renderer::Renderer() {}

void Renderer::Clear() const { /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    /* Draw */
    shader.Bind();
    // shader.SetUniform4f("u_Color", 0.5f, 0.3f, 0.8f, 1.0f);
    va.Bind();
    ib.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}