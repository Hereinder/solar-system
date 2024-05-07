#include "TreeObject.hpp"

TreeObject::~TreeObject() { std::cout << "TreeObject Destructor \n"; };

void TreeObject::LoadMesh() {
    positions = std::vector<float>{
        // positions 2    // texture2
        0.0f,   0.0f,   0.0f,   0.0f, 0.0f,  //
        100.0f, 0.0f,   0.0f,   1.0f, 0.0f,  //
        100.0f, 100.0f, 0.0f,   1.0f, 1.0f,  //
        0.0f,   100.0f, 0.0f,   0.0f, 1.0f,  //

        0.0f,   0.0f,   100.0f, 0.0f, 0.0f,  //
        100.0f, 0.0f,   100.0f, 1.0f, 0.0f,  //
        100.0f, 100.0f, 100.0f, 1.0f, 1.0f,  //
        0.0f,   100.0f, 100.0f, 0.0f, 1.0f,  //

        200.0f, 0.0f,   0.0f,   0.0f, 0.0f,  //
        300.0f, 0.0f,   0.0f,   1.0f, 0.0f,  //
        300.0f, 100.0f, 0.0f,   1.0f, 1.0f,  //
        200.0f, 100.0f, 0.0f,   0.0f, 1.0f,  //

        200.0f, 0.0f,   100.0f, 0.0f, 0.0f,  //
        300.0f, 0.0f,   100.0f, 1.0f, 0.0f,  //
        300.0f, 100.0f, 100.0f, 1.0f, 1.0f,  //
        200.0f, 100.0f, 100.0f, 0.0f, 1.0f,  //

    };

    // indices = new unsigned int[]{0, 1, 2, 2, 3, 0, 5, 6, 7, 7, 4, 5, 8, 9, 10, 10, 11, 8, 13, 14, 15, 15, 12, 13};
    indices =
        std::vector<unsigned int>{0, 1, 2, 2, 3, 0, 5, 6, 7, 7, 4, 5, 8, 9, 10, 10, 11, 8, 13, 14, 15, 15, 12, 13};
    layout.Push<float>(3);
    layout.Push<float>(2);
    m_vbo = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(float));
    // m_ib = std::unique_ptr<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
    m_vao->AddBuffer(*m_vbo, layout);

    m_Texture = std::make_unique<Texture>(m_TexturePath);
    m_Shader = std::make_unique<Shader>(m_ShaderPath);

    m_Shader->Bind();
    m_ib = std::make_unique<IndexBuffer>(indices.data(), indices.size());
    m_Shader->SetUniform1i("u_Texture", 0);
    // m_Texture->Bind();

    /////////////CLEAN/////////////
    m_Shader->Unbind();
    m_Texture->Unbind();
    m_vao->UnBind();
    m_ib->Unbinde();
    m_vbo->Unbinde();
};

void TreeObject::Draw(const glm::mat4&, const glm::mat4&, const glm::vec3&) {
    glm::vec3 translation(300, 300, 0);
    glm::mat4 proj;
    glm::mat4 view;
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    glm::mat4 mvp;
    // glm::mat4 rot0 = glm::mat4(1.0f);
    {
        proj = glm::ortho(0.0f, float(640), 0.0f, float(480), -850.0f, 1550.0f);
        view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        // view = camera.GetViewMatrix();

        // auto rotX = glm::rotate(rot0, to_radian(ang[0]), glm::vec3(1, 0, 0));
        // auto rotY = glm::rotate(rotX, to_radian(ang[1]), glm::vec3(0, 1, 0));
        // model = glm::translate(glm::mat4(1.0f), translation) * glm::rotate(rotY, to_radian(ang[2]), glm::vec3(0, 0,
        // 1));
        model = glm::translate(glm::mat4(1.0f), translation);

        mvp = proj * view * model;
    }

    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);
    m_ib->Bind();
    m_vao->Bind();
    m_Texture->Bind();

    GLCall(glDrawElements(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_INT, nullptr));
};

void TreeObject::Bind() {}
