#pragma once
#include <vector>

#include "Indexbuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

class StaticObject;

class Renderer {
public:
    Renderer();
    void PushBack(StaticObject* object);
    void LoadMesh();

    void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos) const;
    void Clear() const;

private:
    std::vector<StaticObject*> m_ObjectList;
};