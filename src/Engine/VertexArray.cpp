#include "VertexArray.hpp"

#include "VertexBuffer.hpp"
#include "glTools.hpp"

VertexArray::VertexArray() : m_ElementsCount(0) { glGenVertexArrays(1, &m_Renderer_Id); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_Renderer_Id); }

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    unsigned int prev_count = m_ElementsCount;

    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i + prev_count));
        GLCall(glVertexAttribPointer(i + prev_count, element.count, element.type, element.normalized,
                                     layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        m_ElementsCount++;
    }
}

void VertexArray::Bind() const { GLCall(glBindVertexArray(m_Renderer_Id)); }
void VertexArray::UnBind() const { GLCall(glBindVertexArray(0)); }