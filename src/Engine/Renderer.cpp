#include "Renderer.hpp"

#include <GL/glew.h>

#include "StaticObject.hpp"

Renderer::Renderer() {}

void Renderer::Clear() const { /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::PushBack(StaticObject* object) { m_ObjectList.push_back(object); }

void Renderer::LoadMesh() {
    for (auto const obj : m_ObjectList) {
        obj->LoadMesh();
    }
}

void Renderer::Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos) const {
    for (auto const obj : m_ObjectList) {
        obj->Bind();
        obj->Draw(proj, view, campos);
    }
}