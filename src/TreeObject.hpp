#pragma once
#include <memory>

#include "StaticObject.hpp"
class TreeObject : public StaticObject {
public:
    TreeObject() = delete;
    TreeObject(const std::string& shaderPath, const std::string& texturePath) : StaticObject(shaderPath, texturePath){};

    ~TreeObject();
    void LoadMesh();
    void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos);
    void Bind();

private:
    std::vector<float> positions;
    std::vector<unsigned int> indices;
};