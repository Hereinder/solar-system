#include "glTools.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void GLClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLuint error = glGetError()) {
        std::cout << "[ERROR OpenGL]: " << error << " | " << file << " : " << function << " : " << line << "\n";
        return false;
    }
    return true;
}