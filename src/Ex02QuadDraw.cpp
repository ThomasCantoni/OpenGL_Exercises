#include "Ex02QuadDraw.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <vector>

void Ex02QuadDraw::Start()
{
    Program = new OGLProgram("resources/shaders/triangle.vert", "resources/shaders/triangle.frag");

    std::vector<float> Vertices = {
        //Left Triangle
        -0.5f,  0.5f, 0.f,  //top left
        -0.5f, -0.5f, 0.f,  //bottom left
         0.5f,  0.5f, 0.f,  //top right

        //Right Triangle
         0.5f,  0.5f, 0.f,  //top right
         0.5f, -0.5f, 0.f,  //bottom right
        -0.5f, -0.5f, 0.f,  //bottom left
    };

    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2. Create VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    //4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();
}

void Ex02QuadDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Ex02QuadDraw::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
}