#pragma once
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <vector>

#include "StaticObject.hpp"

class InstancingObject : public StaticObject {
public:
    InstancingObject(const std::string& shaderPath = "", const std::string& texturePath = "");
    void LoadMesh();
    void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos);
    void Bind();

private:
    void UnBind();
    void ModifyElement(int offset, int count);
    void AliveElement(int offset, int count);

private:
    std::vector<float> m_VertexData;
    std::vector<unsigned int> m_Indices;
    glm::vec3 m_Translation;
    glm::mat4 model;
    VertexBufferLayout layoutTransform;
    std::unique_ptr<VertexBuffer> m_vboTransform;
    std::vector<glm::mat4> modelMatrices;

    bool m_TextureAvailable;
    std::shared_mutex m_Mutex;
};
