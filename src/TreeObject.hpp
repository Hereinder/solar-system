#pragma once
#include <memory>

#include "StaticObject.hpp"
class TreeObject : public StaticObject {
public:
    TreeObject() = delete;
    TreeObject(const std::string& shaderPath, const std::string& texturePath) : StaticObject(shaderPath, texturePath){};

    ~TreeObject();
    void LoadMesh();
    void Draw(float* ang);

private:
    std::vector<float> positions;
    std::vector<unsigned int> indices;
};