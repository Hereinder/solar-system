#pragma once
#include <memory>

#include "StaticObject.hpp"
class StoneObject : public StaticObject {
public:
    StoneObject() = delete;
    StoneObject(glm::vec3 trans, const std::string& shaderPath = "", const std::string& texturePath = "")
        : StaticObject(shaderPath, texturePath), m_Translation(trans) {
        m_TextureAvailable = texturePath.size() > 0 ? true : false;
        Batch(trans.x, trans.y, trans.z, 45);
    };

    ~StoneObject();
    void LoadMesh();
    void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos);
    void Bind();
    void Batch(float x, float y, float z, float angle);
    void UnBind();

private:
    std::vector<float> m_VertexData;
    std::vector<unsigned int> m_Indices;
    glm::vec3 m_Translation;
    glm::mat4 model;

    bool m_TextureAvailable;
};