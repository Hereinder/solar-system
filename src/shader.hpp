#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <unordered_map>
struct ShaderSource {
    std::string VertexShader;
    std::string FragmentShader;
};

class Shader {
public:
    Shader(const std::string& fileName);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // uniforms
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform2f(const std::string& name, float v0, float v1);
    void SetUniform1f(const std::string& name, float v0);

    void SetUniform1i(const std::string& name, int v0);

    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    ShaderSource ParseShader();
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);

private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};