#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Indexbuffer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "glTools.hpp"
#include "shader.hpp"

GLFWwindow* init() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "ERROR: glfwInit() \n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);
    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: glewInit() \n";
    }
    return window;
}

void rotateTriangle(unsigned int shader) {
    // create transformations
    glm::mat4 transform = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
    // transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

    unsigned int transformLoc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

int main(void) {
    GLFWwindow* window = init();

    unsigned int shader1, shader2;

    ////////////////////////////////
    float positions[] = {
        // positions         // colors
        -0.0f, -0.0f, 0.0,  // 0
        1.0f,  -0.0f, 0.0,  // 1
        1.0f,  1.0f,  0.0,  // 2
        -0.0f, 5.0f,  0.0,  // 3

    };

    float positions2[] = {
        // positions         // colors
        -1.0f, -0.0f, 0.0,  // 0
        0.5f,  -0.0f, 0.0,  // 1
        0.5f,  1.0f,  0.0,  // 2
        -0.0f, 5.0f,  0.0,  // 3

    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VertexArray va;
    VertexBuffer vb(positions, sizeof(positions));
    VertexBuffer vb2(positions2, sizeof(positions2));

    VertexBufferLayout layout;
    layout.Push<float>(3);

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);
    ib.Unbinde();
    vb.Unbinde();

    ////////////////////////////////

    GLCall(shader1 = GLLoadShader());
    GLCall(shader2 = GLLoadShader());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    int location1 = glGetUniformLocation(shader1, "u_Color");
    int location2 = glGetUniformLocation(shader2, "u_Color");

    ASSERT(location1 != -1);
    ASSERT(location2 != -1);

    /* Loop until the user closes the window */
    float r = 0;
    float increment = 0.01;
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*---------------------------- */
        // rotateTriangle(shader1);
        if (r >= 1.0f || r < 0.0f) {
            increment = increment * -1.0;
        }
        r += increment;
        /*---------------------------- */

        /* Draw */
        GLCall(glUseProgram(shader1));
        GLCall(glUniform4f(location1, r, 1.0f, 0.0f, 1.0f));

        va.Bind();
        ib.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /*---------------------------- */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}