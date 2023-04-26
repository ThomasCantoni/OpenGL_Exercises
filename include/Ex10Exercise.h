#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>

class Ex10Exercise 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    OGLProgram* Program;
    OGLProgram* CubeProgram;
    GLuint Vao;
    GLuint CubeVao;
    GLuint Vbo;
    GLuint CubeVbo;
    OGLTexture* StormText;
    glm::vec3 PointLightPos;
    glm::mat4 View;
    glm::mat4 Projection;
    int VerticeCount;
    int CubeVerticeCount;
};