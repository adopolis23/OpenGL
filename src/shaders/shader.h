#pragma once
#include <string>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void bind();

    void setMat4(const std::string& name, const glm::mat4& m);

    unsigned int id;

private:

    // loads a string from a filepath
    std::string LoadShaderFromFile(const std::string& path);

};
