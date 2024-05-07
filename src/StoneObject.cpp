#include "StoneObject.hpp"

StoneObject::~StoneObject() { std::cout << "StoneObject Destructor \n"; };

void StoneObject::Batch(float x, float y, float z, float angle) {
    std::vector<float> single{
        //         positions 3             // texture 2  // normals 3
        x + -50.0f, y + -50.0f, z + -50.0f, 0.0f, 0.0f, 0.0f,   0.0f,   -50.0f,  //
        x + 50.0f,  y + -50.0f, z + -50.0f, 1.0f, 0.0f, 0.0f,   0.0f,   -50.0f,  //
        x + 50.0f,  y + 50.0f,  z + -50.0f, 1.0f, 1.0f, 0.0f,   0.0f,   -50.0f,  //
        x + -50.0f, y + 50.0f,  z + -50.0f, 0.0f, 1.0f, 0.0f,   0.0f,   -50.0f,  //

        x + -50.0f, y + -50.0f, z + 50.0f,  0.0f, 0.0f, 0.0f,   0.0f,   50.0f,  //
        x + 50.0f,  y + -50.0f, z + 50.0f,  1.0f, 0.0f, 0.0f,   0.0f,   50.0f,  //
        x + 50.0f,  y + 50.0f,  z + 50.0f,  1.0f, 1.0f, 0.0f,   0.0f,   50.0f,  //
        x + -50.0f, y + 50.0f,  z + 50.0f,  0.0f, 1.0f, 0.0f,   0.0f,   50.0f,  //

        x + -50.0f, y + 50.0f,  z + 50.0f,  1.0f, 0.0f, -50.0f, 0.0f,   0.0f,  //
        x + -50.0f, y + 50.0f,  z + -50.0f, 1.0f, 1.0f, -50.0f, 0.0f,   0.0f,  //
        x + -50.0f, y + -50.0f, z + -50.0f, 0.0f, 1.0f, -50.0f, 0.0f,   0.0f,  //
        x + -50.0f, y + -50.0f, z + 50.0f,  0.0f, 0.0f, -50.0f, 0.0f,   0.0f,  //

        x + 50.0f,  y + 50.0f,  z + 50.0f,  1.0f, 0.0f, 50.0f,  0.0f,   0.0f,  //
        x + 50.0f,  y + 50.0f,  z + -50.0f, 1.0f, 1.0f, 50.0f,  0.0f,   0.0f,  //
        x + 50.0f,  y + -50.0f, z + -50.0f, 0.0f, 1.0f, 50.0f,  0.0f,   0.0f,  //
        x + 50.0f,  y + -50.0f, z + 50.0f,  0.0f, 0.0f, 50.0f,  0.0f,   0.0f,  //

        x + -50.0f, y + -50.0f, z + -50.0f, 0.0f, 1.0f, 0.0f,   -50.0f, 0.0f,  //
        x + 50.0f,  y + -50.0f, z + -50.0f, 1.0f, 1.0f, 0.0f,   -50.0f, 0.0f,  //
        x + 50.0f,  y + -50.0f, z + 50.0f,  1.0f, 0.0f, 0.0f,   -50.0f, 0.0f,  //
        x + -50.0f, y + -50.0f, z + 50.0f,  0.0f, 0.0f, 0.0f,   -50.0f, 0.0f,  //

        x + -50.0f, y + 50.0f,  z + -50.0f, 0.0f, 1.0f, 0.0f,   50.0f,  0.0f,  //
        x + 50.0f,  y + 50.0f,  z + -50.0f, 1.0f, 1.0f, 0.0f,   50.0f,  0.0f,  //
        x + 50.0f,  y + 50.0f,  z + 50.0f,  1.0f, 0.0f, 0.0f,   50.0f,  0.0f,  //
        x + -50.0f, y + 50.0f,  z + 50.0f,  0.0f, 0.0f, 0.0f,   50.0f,  0.0f,  //
    };
    positions.insert(positions.end(), single.begin(), single.end());

    unsigned int nb = positions.size() / 120;
    std::vector<unsigned int> ind{0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                                  12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

    for (unsigned int& d : ind) d += (nb - 1) * 24;
    indices.insert(indices.end(), ind.begin(), ind.end());
}

void StoneObject::LoadMesh() {
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);

    m_vbo = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(float));
    // m_ib = std::unique_ptr<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
    m_vao->AddBuffer(*m_vbo, layout);
    m_Shader = std::make_unique<Shader>(m_ShaderPath);
    m_Shader->Bind();
    m_ib = std::make_unique<IndexBuffer>(indices.data(), indices.size());

    if (m_TextureAvailable) {
        m_Texture = std::make_unique<Texture>(m_TexturePath);
        m_Shader->SetUniform1i("u_Texture", 0);
    }

    /////////////CLEAN/////////////
    UnBind();
};

void StoneObject::Bind() {
    m_Shader->Bind();
    m_vao->Bind();

    if (m_TextureAvailable) {
        m_Texture->Bind();
    }
}

void StoneObject::UnBind() {
    m_Shader->Unbind();
    if (m_TextureAvailable) {
        m_Texture->Unbind();
    }

    m_vao->UnBind();
    m_ib->Unbinde();
    m_vbo->Unbinde();
}

void StoneObject::Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& campos) {
    Bind();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 mvp;
    glm::mat4 rot0 = glm::mat4(1.0f);
    {
        model = glm::rotate(model, (float)glfwGetTime() / 10, glm::vec3(0.0f, 1.0f, 0.0f));
        mvp = proj * view * model;
    }

    m_Shader->SetUniformMat4f("u_Model", model);
    m_Shader->SetUniformMat4f("u_View", view);
    m_Shader->SetUniformMat4f("u_Projection", proj);

    m_Shader->SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
    m_Shader->SetUniform3f("u_ViewPos", campos.x, campos.y, campos.z);
    m_Shader->SetUniform3f("u_LightPos", 0.0f, 0.0f, 0.0f);
    m_Shader->SetUniform3f("u_Color", 1.0f, 0.5f, 0.31f);

    GLCall(glDrawElements(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_INT, nullptr));
    UnBind();
};