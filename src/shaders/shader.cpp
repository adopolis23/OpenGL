#include "shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {

    auto compile = [](const char* src, GLenum type) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, 0);
        glCompileShader(shader);
        return shader;
    };

    GLuint v = compile(vertexSrc.c_str(), GL_VERTEX_SHADER);
    GLuint f = compile(fragmentSrc.c_str(), GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, v);
    glAttachShader(id, f);
    glLinkProgram(id);

    glDeleteShader(v);
    glDeleteShader(f);
}

void Shader::bind() {
    glUseProgram(id);
}

void Shader::setMat4(const std::string& name, const glm::mat4& m) {
    GLint loc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}
