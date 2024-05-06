#pragma once
#include <memory>

#include "StaticObject.hpp"
class StaticLight : public StaticObject {
public:
    StaticLight() = delete;
    StaticLight(glm::vec3 trans, const std::string& shaderPath, const std::string& texturePath = "")
        : StaticObject(shaderPath, texturePath), translation(trans) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        Batch(0, 0, 0);
    };

    ~StaticLight();
    void LoadMesh();
    void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos );
    void Bind();
    void Batch(float x, float y, float z);
    void UnBind();

private:
    std::vector<float> positions;
    std::vector<unsigned int> indices;
    float anglx = 30;
    float angly = 30;
    float anglz = 0;
    int cnt = 0;
    glm::vec3 translation;
    glm::mat4 model;
};