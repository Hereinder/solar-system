#include "StaticObject.hpp"

StaticObject::StaticObject(const std::string& shaderPath, const std::string& texturePath)
    : m_ShaderPath(shaderPath), m_TexturePath(texturePath) {
    m_vao = std::make_unique<VertexArray>();
}