#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"

class Ex07CubePerspDraw 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    OGLProgram* Program;
    GLuint Vao;
    GLuint Vbo;
    OGLTexture* WoodText;
};