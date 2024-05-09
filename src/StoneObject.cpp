#include "StoneObject.hpp"

StoneObject::~StoneObject() { std::cout << "StoneObject Destructor \n"; };

void StoneObject::Batch(float x, float y, float z, float angle) {
    std::array<float, 192> single_vertex{
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

    std::array<unsigned int, 36> single_indices{0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                                                12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};
    // std::vector<float> rotatedVertices;

    // {
    //     std::vector<glm::vec3> vertices;
    //     for (size_t i = 0; i < single.size(); i += 8) {
    //         vertices.emplace_back(single[i], single[i + 1], single[i + 2]);
    //     }
    //     // Create a glm::mat4 matrix
    //     glm::mat4 transformMatrix(1.0f);  // Initialize as an identity matrix

    //     // Apply rotation to the transformation matrix
    //     glm::vec3 axis(0.0f, 1.0f, 0.0f);  // Rotation axis, e.g., y-axis
    //     transformMatrix = glm::rotate(transformMatrix, glm::radians(angle), axis);
    //     // Transform the vertices
    //     std::vector<glm::vec3> transformedVertices;
    //     transformedVertices.reserve(vertices.size());
    //     for (const auto& vertex : vertices) {
    //         // Transform each vertex by multiplying with the transformation matrix
    //         glm::vec4 transformedVertex = transformMatrix * glm::vec4(vertex, 1.0f);
    //         transformedVertices.push_back(glm::vec3(transformedVertex));  // Convert back to vec3
    //     }
    //     // To retrieve the data back to a std::vector<float>, you can simply copy it
    //     rotatedVertices.reserve(transformedVertices.size() * 3);  // Reserve space for vertices
    //     int cnt = 0;
    //     for (const auto& vertex : transformedVertices) {
    //         rotatedVertices.push_back(vertex.x);
    //         rotatedVertices.push_back(vertex.y);
    //         rotatedVertices.push_back(vertex.z);
    //         rotatedVertices.insert(rotatedVertices.end(), single.begin() + cnt + 3, single.begin() + cnt + 8);
    //         cnt += 8;
    //     }
    // }

    m_VertexData.insert(m_VertexData.end(), single_vertex.begin(), single_vertex.end());
    unsigned int positions_count = m_VertexData.size() / single_vertex.size();
    unsigned int indices_count = *std::max_element(single_indices.begin(), single_indices.end()) + 1;

    for (unsigned int& d : single_indices) d += (positions_count - 1) * indices_count;

    m_Indices.insert(m_Indices.end(), single_indices.begin(), single_indices.end());
}

void StoneObject::LoadMesh() {
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);

    m_vbo = std::make_unique<VertexBuffer>(m_VertexData.data(), m_VertexData.size() * sizeof(float));
    // m_ib = std::unique_ptr<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
    m_vao->AddBuffer(*m_vbo, layout);
    m_Shader = std::make_unique<Shader>(m_ShaderPath);
    m_Shader->Bind();
    m_ib = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size());

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
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
    glm::mat4 mvp;
    // glm::mat4 rot0 = glm::mat4(1.0f);
    {
        // model = glm::rotate(model, (float)glfwGetTime() / 10, glm::vec3(0.0f, 1.0f, 0.0f));
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