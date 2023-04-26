#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <vector>

struct QuadInstance 
{
    glm::vec3 Position;
    float Rotation;
    glm::vec3 Scale;
};

class Ex09InstancingDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    GLuint Ebo;
    OGLTexture* SmileText;
    OGLTexture* WoodText;
    glm::mat4 View;
    glm::mat4 Projection;
    std::vector<QuadInstance> Quads;
    GLuint VboMvp;
    std::vector<glm::mat4> MvpData;
};