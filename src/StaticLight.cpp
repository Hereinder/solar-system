#include "StaticLight.hpp"

StaticLight::~StaticLight() { std::cout << "StaticLight Destructor \n"; };

void StaticLight::Batch(float x, float y, float z) {
    std::vector<float> single{
        // m_VertexData 2    // texture2
        x + -50.0f, y + -50.0f, z + -50.0f,  //
        x + 50.0f,  y + -50.0f, z + -50.0f,  //
        x + 50.0f,  y + 50.0f,  z + -50.0f,  //
        x + -50.0f, y + 50.0f,  z + -50.0f,  //

        x + -50.0f, y + -50.0f, z + 50.0f,  //
        x + 50.0f,  y + -50.0f, z + 50.0f,  //
        x + 50.0f,  y + 50.0f,  z + 50.0f,  //
        x + -50.0f, y + 50.0f,  z + 50.0f,  //

        x + -50.0f, y + 50.0f,  z + 50.0f,   //
        x + -50.0f, y + 50.0f,  z + -50.0f,  //
        x + -50.0f, y + -50.0f, z + -50.0f,  //
        x + -50.0f, y + -50.0f, z + 50.0f,   //

        x + 50.0f,  y + 50.0f,  z + 50.0f,   //
        x + 50.0f,  y + 50.0f,  z + -50.0f,  //
        x + 50.0f,  y + -50.0f, z + -50.0f,  //
        x + 50.0f,  y + -50.0f, z + 50.0f,   //

        x + -50.0f, y + -50.0f, z + -50.0f,  //
        x + 50.0f,  y + -50.0f, z + -50.0f,  //
        x + 50.0f,  y + -50.0f, z + 50.0f,   //
        x + -50.0f, y + -50.0f, z + 50.0f,   //

        x + -50.0f, y + 50.0f,  z + -50.0f,  //
        x + 50.0f,  y + 50.0f,  z + -50.0f,  //
        x + 50.0f,  y + 50.0f,  z + 50.0f,   //
        x + -50.0f, y + 50.0f,  z + 50.0f,   //
    };
    m_VertexData.insert(m_VertexData.end(), single.begin(), single.end());
    unsigned int nb = m_VertexData.size() / single.size();
    std::vector<unsigned int> ind{0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                                  12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

    for (unsigned int& d : ind) d += (nb - 1) * ind.size();
    m_Indices.insert(m_Indices.end(), ind.begin(), ind.end());
}

void StaticLight::LoadMesh() {
    layout.Push<float>(3);
    m_vbo = std::make_unique<VertexBuffer>(m_VertexData.data(), m_VertexData.size() * sizeof(float));
    m_vao->AddBuffer(*m_vbo, layout);

    m_Shader = std::make_unique<Shader>(m_ShaderPath);
    m_Shader->Bind();

    m_ib = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size());

    /////////////CLEAN/////////////
    UnBind();
};

void StaticLight::Bind() {
    m_Shader->Bind();
    m_vao->Bind();
}
void StaticLight::UnBind() {
    m_vbo->Unbinde();
    m_vao->UnBind();
    m_Shader->Unbind();
    m_ib->Unbinde();
}

void StaticLight::Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3&) {
    Bind();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);

    glm::mat4 mvp;
    // glm::mat4 rot0 = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() / 10, glm::vec3(0.0f, 1.0f, 0.0f));
    mvp = proj * view * model;

    m_Shader->SetUniformMat4f("u_MVP", mvp);
    GLCall(glDrawElements(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_INT, nullptr));
    UnBind();
};