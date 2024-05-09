#pragma once
#include <algorithm>
#include <array>
#include <iostream>  // TODO
#include <memory>
#include <string>

#include "Camera.hpp"
#include "Indexbuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "timer.hpp"

class StaticObject {
public:
    StaticObject(const std::string& shaderPath = "", const std::string& texturePath = "");
    StaticObject() = delete;

    virtual ~StaticObject() { std::cout << "StaticObject Destructor \n"; };
    virtual void LoadMesh() = 0;
    virtual void Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos) = 0;
    virtual void Bind() = 0;
    void setInstancingStep(int step) { m_InstancingStep = step; }

protected:
    std::string m_ShaderPath;
    std::string m_TexturePath;

    VertexBufferLayout layout;
    std::unique_ptr<VertexBuffer> m_vbo;
    std::unique_ptr<VertexArray> m_vao;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;
    std::unique_ptr<IndexBuffer> m_ib;

    int m_InstancingStep = 10000;
    Timer m_ObjectTimer;
};