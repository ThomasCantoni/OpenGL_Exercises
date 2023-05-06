#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"

class Ex11PostFX 
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

    GLuint SceneFbo;
    GLuint SceneTexture;
    GLuint SceneRbo;

    GLuint QuadVao;
    GLuint QuadVbo;
    OGLProgram* QuadProgram;
};