#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    void bind();

    void setMat4(const std::string& name, const glm::mat4& m);

    unsigned int id;
};
