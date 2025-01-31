#include "InstancingObject.hpp"

#include "Gui.hpp"

InstancingObject::InstancingObject(const std::string& shaderPath, const std::string& texturePath)
    : StaticObject(shaderPath, texturePath) {
    m_vao = std::make_unique<VertexArray>();

    std::array<float, 72> single_vertex{
        //         m_VertexData 3
        -50.0f, -50.0f, -50.0f,  //
        50.0f,  -50.0f, -50.0f,  //
        50.0f,  50.0f,  -50.0f,  //
        -50.0f, 50.0f,  -50.0f,  //

        -50.0f, -50.0f, 50.0f,  //
        50.0f,  -50.0f, 50.0f,  //
        50.0f,  50.0f,  50.0f,  //
        -50.0f, 50.0f,  50.0f,  //

        -50.0f, 50.0f,  50.0f,   //
        -50.0f, 50.0f,  -50.0f,  //
        -50.0f, -50.0f, -50.0f,  //
        -50.0f, -50.0f, 50.0f,   //

        50.0f,  50.0f,  50.0f,   //
        50.0f,  50.0f,  -50.0f,  //
        50.0f,  -50.0f, -50.0f,  //
        50.0f,  -50.0f, 50.0f,   //

        -50.0f, -50.0f, -50.0f,  //
        50.0f,  -50.0f, -50.0f,  //
        50.0f,  -50.0f, 50.0f,   //
        -50.0f, -50.0f, 50.0f,   //

        -50.0f, 50.0f,  -50.0f,  //
        50.0f,  50.0f,  -50.0f,  //
        50.0f,  50.0f,  50.0f,   //
        -50.0f, 50.0f,  50.0f,   //
    };
    std::array<unsigned int, 36> single_indices{0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
                                                12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

    m_VertexData.insert(m_VertexData.end(), single_vertex.begin(), single_vertex.end());
    unsigned int positions_count = m_VertexData.size() / single_vertex.size();
    unsigned int indices_count = *std::max_element(single_indices.begin(), single_indices.end()) + 1;

    for (unsigned int& d : single_indices) d += (positions_count - 1) * indices_count;

    m_Indices.insert(m_Indices.end(), single_indices.begin(), single_indices.end());

    rebuild();
}

void InstancingObject::Draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3&) {
    static int j = 0;
    int step = 0;
    int remainingElements = 0;
    m_ObjectTimer.reset();
    static int InstanceSize = Gui::GetInstance().m_InstanceSize;
    std::vector<std::thread> taskContainer;
    m_InstancingStep = Gui::GetInstance().m_InstanceStep;

    if (InstanceSize != Gui::GetInstance().m_InstanceSize) {
        rebuild();
        InstanceSize = Gui::GetInstance().m_InstanceSize;
    }

    // std::cout << "0: " << m_ObjectTimer.tick().us().count() << "\n";

    for (int i = 0; j < modelMatrices.size();) {
        remainingElements = modelMatrices.size() - j;
        step = std::min(m_InstancingStep, remainingElements);
        taskContainer.emplace_back(std::thread(InstancingObject::ModifyElement, this, i, step));
        j += step;
        i += step;
    }

    for (auto& task : taskContainer) {
        task.join();
    }

    // std::cout << "1: " << m_ObjectTimer.tick().us().count() << "\n";

    if (j >= modelMatrices.size())
        j = 0;

    m_vboTransform->Update(modelMatrices.data(), sizeof(glm::mat4) * modelMatrices.size());
    // m_vboTransform->SubUpdate(&modelMatrices[j], sizeof(glm::mat4) * step, j);

    {
        Bind();
        glm::mat4 mvp = proj * view;
        m_Shader->SetUniformMat4f("u_Model", mvp);
        GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_ib->GetCount(), GL_UNSIGNED_INT, 0, modelMatrices.size() * 2));
        UnBind();
    }
    // std::cout << "2: " << m_ObjectTimer.tick().us().count() << "\n";
}

void InstancingObject::LoadMesh() {
    m_vbo = std::make_unique<VertexBuffer>(m_VertexData.data(), m_VertexData.size() * sizeof(float));
    layout.Push<float>(3);
    m_vao->AddBuffer(*m_vbo, layout);

    m_vboTransform = std::make_unique<VertexBuffer>(&modelMatrices[0], sizeof(glm::mat4) * modelMatrices.size());
    layoutTransform.Push<float>(4);
    layoutTransform.Push<float>(4);
    layoutTransform.Push<float>(4);
    layoutTransform.Push<float>(4);
    m_vao->AddBuffer(*m_vboTransform, layoutTransform);
    GLCall(glVertexAttribDivisor(1, 2));
    GLCall(glVertexAttribDivisor(2, 2));
    GLCall(glVertexAttribDivisor(3, 2));
    GLCall(glVertexAttribDivisor(4, 2));

    m_Shader = std::make_unique<Shader>(m_ShaderPath);
    m_Shader->Bind();
    m_ib = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size());

    /////////////CLEAN/////////////
    UnBind();
};

void InstancingObject::Bind() {
    m_Shader->Bind();
    m_vao->Bind();
    m_vbo->Bind();
    m_ib->Bind();
    m_vboTransform->Bind();
}

void InstancingObject::UnBind() {
    m_Shader->Unbind();
    m_vao->UnBind();
    m_ib->Unbinde();
    m_vbo->Unbinde();
    m_vboTransform->Unbinde();
}

void InstancingObject::ModifyElement(int offset, int count) {
    for (int i = offset; i < offset + count; i++) {
        modelMatrices[i] = glm::translate(modelMatrices[i], glm::vec3(0.0, 50.0, 0.0));
    }
    // std::unique_lock lock(m_Mutex);
}

void InstancingObject::AliveElement(int offset, int count) {
    for (int i = offset; i < offset + count; i++) {
        // modelMatrices[i].value.x;
    }
    // std::unique_lock lock(m_Mutex);
}

void InstancingObject::rebuild() {
    modelMatrices.clear();

    // generate a large list of semi-random model transformation matrices
    // ------------------------------------------------------------------
    {
        srand(static_cast<unsigned int>(glfwGetTime()));  // initialize random seed
        float radius = 2000.0;
        float offset = 500.0f;
        for (int i = 0; i < Gui::GetInstance().m_InstanceSize * 1000; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            // 1. m_Translation: displace along circle with 'radius' in range [-offset, offset]
            float angle = (float)i / (float)(Gui::GetInstance().m_InstanceSize * 1000) * 360.0f;
            float displacement = (rand() % (int)(2 * offset)) - offset;
            float x = sin(angle) * radius + displacement;
            displacement = (rand() % (int)(2 * offset)) - offset;
            float y =
                cos(angle) * radius + displacement;  // keep height of asteroid field smaller compared to width of x and
            displacement = (rand() % (int)(2 * offset)) - offset;
            float z = cos(angle) * radius + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. scale: Scale between 0.05 and 0.25f
            float scale = static_cast<float>((rand() % 10) / 100.0 + 0.02);
            model = glm::scale(model, glm::vec3(scale));

            // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
            float rotAngle = static_cast<float>((rand() % 360));
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. now add to list of matrices
            modelMatrices.push_back(model);
        }
    }

    // for (int i = 0; i < Gui::GetInstance().m_InstanceSize; i++) {
    //     for (int k = 0; k < Gui::GetInstance().m_InstanceSize; k++) {
    //         glm::mat4 model = glm::mat4(1.0f);
    //         model = glm::translate(model, glm::vec3(i, 1, k));
    //         model = glm::scale(model, glm::vec3(0.009));
    //         // 4. now add to list of matrices
    //         modelMatrices2.push_back(model);
    //     }
    // }
}