#pragma once
#include "Indexbuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

class Renderer {
public:
    Renderer();
    void Draw(VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;

private:
};