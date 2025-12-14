#include "shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

    std::string vertexSrc = LoadShaderFromFile(vertexPath);
    std::string fragmentSrc = LoadShaderFromFile(fragmentPath);

    auto compile = [](const char* src, GLenum type) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, 0);
        glCompileShader(shader);

        // Check compile status
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
        }

        return shader;
    };

    printf("Compiling vertex shader\n");
    GLuint v = compile(vertexSrc.c_str(), GL_VERTEX_SHADER);

    printf("Compiling fragment shader\n");
    GLuint f = compile(fragmentSrc.c_str(), GL_FRAGMENT_SHADER);

    printf("Linking shaders\n");
    id = glCreateProgram();
    glAttachShader(id, v);
    glAttachShader(id, f);
    glLinkProgram(id);

    glDeleteShader(v);
    glDeleteShader(f);
}

void Shader::bind() {
    //printf("Binding Shader Id: %d\n", id);
    glUseProgram(id);
}

void Shader::setMat4(const std::string& name, const glm::mat4& m) {
    GLint loc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setInt(const std::string& name, int value)
{
    GLint loc = glGetUniformLocation(id, name.c_str());
    glUniform1i(loc, value);
}


// loads shader from file, maybe this should be a general util function for loading strings from files..
std::string Shader::LoadShaderFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf(); // read entire file into stringstream
    return buffer.str();    // convert to std::string
}