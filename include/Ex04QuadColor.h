#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"

class Ex04QuadColor 
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
};