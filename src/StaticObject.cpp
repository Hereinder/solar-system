#include "StaticObject.hpp"

StaticObject::StaticObject(const std::string& shaderPath, const std::string& texturePath)
    : m_ShaderPath(shaderPath), m_TexturePath(texturePath) {
    m_vao = std::make_unique<VertexArray>();
    // m_vao->AddBuffer(m_vbo, layout);

    // m_Shader = std::make_unique<Shader>(m_ShaderPath);
    // m_Texture = std::make_unique<Texture>(m_TexturePath);
}