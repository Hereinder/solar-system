#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) \
    if (!(x)) raise(SIGTRAP);

#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLuint error = glGetError()) {
        std::cout << "[ERROR OpenGL]: " << error << " | " << file << " : " << function << " : " << line << "\n";
        return false;
    }
    return true;
}