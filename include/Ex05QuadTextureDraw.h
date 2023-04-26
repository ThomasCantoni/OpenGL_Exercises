#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"

class Ex05QuadTextureDraw 
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
};