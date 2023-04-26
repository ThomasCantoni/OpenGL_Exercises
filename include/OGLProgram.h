#pragma once
#include <string>
#include <glad/glad.h>
#include "Common.h"
#include <glm/glm.hpp>

class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertShaderPath, const std::string& InFragShaderPath);
    ~OGLProgram();
    void Bind();
    GLuint ID();
    void SetUniform(const std::string& name, const Color& color);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, glm::mat4 matrix);
    void SetUniform(const std::string& name, glm::vec3 vect);
private:
    GLuint ProgramId;
};