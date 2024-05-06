#include "Shader.hpp"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <sstream>

#include "glTools.hpp"

Shader::Shader(const std::string& fileName) : m_FilePath(fileName), m_RendererID(0) {
    ShaderSource shaderSource = ParseShader();
    m_RendererID = CreateShader(shaderSource.VertexShader, shaderSource.FragmentShader);
}

Shader::~Shader() { GLCall(glDeleteProgram(m_RendererID)); }

ShaderSource Shader::ParseShader() {
    std::ifstream stream(m_FilePath);
    if (stream.fail()) {
        std::cout << "[ERROR OpenGL:Shader] : input file parsing error\n";
    }
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }
    return {ss[static_cast<int>(ShaderType::VERTEX)].str(), ss[static_cast<int>(ShaderType::FRAGMENT)].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

void Shader::Bind() const { GLCall(glUseProgram(m_RendererID)); }
void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1) {
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

void Shader::SetUniform1f(const std::string& name, float v0) { GLCall(glUniform1f(GetUniformLocation(name), v0)); }

void Shader::SetUniform1i(const std::string& name, int v0) { GLCall(glUniform1i(GetUniformLocation(name), v0)); }

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)));
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    int location = 0;
    GLCall(location = glGetUniformLocation(m_RendererID, name.c_str()));

    if (location == -1) {
        std::cout << "[WARNING OpenGL:Shader] : uniform " << name << " doesnt exist. Creating! \n";
    }
    m_UniformLocationCache[name] = location;
    return location;
}
