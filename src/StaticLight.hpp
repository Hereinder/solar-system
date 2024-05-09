#pragma once
#include <memory>

#include "StaticObject.hpp"
class StaticLight : public StaticObject {
public:
    StaticLight() = delete;
    StaticLight(glm::vec3 trans, const std::string& shaderPath, const std::string& texturePath = "")
        : StaticObject(shaderPath, texturePath), m_Translation(trans) {
        Batch(0, 0, 0);
    };

    ~StaticLight();
    void LoadMesh();
    void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos);
    void Bind();
    void Batch(float x, float y, float z);
    void UnBind();

private:
    std::vector<float> m_VertexData;
    std::vector<unsigned int> m_Indices;
    float anglx = 30;
    float angly = 30;
    float anglz = 0;
    int cnt = 0;
    glm::vec3 m_Translation;
    glm::mat4 model;
};