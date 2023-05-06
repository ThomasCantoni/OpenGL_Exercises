#pragma once
#include <vector>
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include "light_t.h"
class Ex14ManyLightsDEFERRED
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    OGLTexture* StormText;
    glm::mat4 View;
    glm::mat4 Projection;
    int VerticeCount;

    GLuint GFbo;
    GLuint DiffuseText;
    GLuint NormalText;
    GLuint PositionText;

    GLuint DepthRbo;
    OGLProgram* QuadProgram;
    GLuint QuadVao;
    GLuint QuadVbo;
    GLuint QuadLightsStruct;
    std::vector<light_t_cpu_side> lights_struct;
};