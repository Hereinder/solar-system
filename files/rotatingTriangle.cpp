#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << "\n";
        glDeleteShader(id);
        return 0;
    }
    // TODO:  handle error
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int executeShader() {
    std::string vertexShader = R"(
        #version 330 core
        layout(location = 0) in vec4 position;
        uniform mat4 transform;

        void main()
        {
            gl_Position = transform * position;
        }
    )";

    std::string fragmentShader = R"(
        #version 330 core
        layout(location = 0) out vec4 color;
        void main()
        {
            color = vec4(1.0, 0.0, 0.0, 1.0f);
        }
    )";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);
    return shader;
}

GLFWwindow* init() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "ERROR: glfwInit() \n";
        return nullptr;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR: glewInit() \n";
    }
    return window;
}

void createTriangle() {
    float positions[] = {
        // positions         // colors
        0.5f,  -0.5f,  // bottom right
        -0.5f, -0.5f,  // bottom left
        0.0f,  0.5f    // top

    };

    unsigned int buffer, VAO;
    // glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &buffer);
    // glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
}

void rotateTriangle(unsigned int shader) {
    // create transformations
    glm::mat4 transform = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

    unsigned int transformLoc = glGetUniformLocation(shader, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

int main(void) {
    GLFWwindow* window = init();

    createTriangle();
    unsigned int shader = executeShader();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*---------------------------- */
        rotateTriangle(shader);
        /*---------------------------- */

        /* Draw */
        glDrawArrays(GL_TRIANGLES, 0, 3);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}