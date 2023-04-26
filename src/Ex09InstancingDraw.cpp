#include "Ex09InstancingDraw.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

void Ex09InstancingDraw::Start()
{
    Program = new OGLProgram("resources/shaders/instancing.vert", "resources/shaders/instancing.frag");
    
    std::vector<float> Vertices = {
        // Positions        // Uvs
        -0.5f,  0.5f, 0.f,  0.f, 1.f, //top left
        -0.5f, -0.5f, 0.f,  0.f, 0.f, //bottom left
         0.5f,  0.5f, 0.f,  1.f, 1.f, //top right    
         0.5f, -0.5f, 0.f,  1.f, 0.f//bottom right
    };

    std::vector<uint32_t> Indexes = {
        0, 1, 2, //Left Triangle
        2, 3, 1, //Right Triangle
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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Crea EBO
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    int EboSize = Indexes.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, EboSize, Indexes.data(), GL_STATIC_DRAW);

    //4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    SmileText = new OGLTexture("resources/textures/smile.png");
    WoodText = new OGLTexture("resources/textures/wood-box.jpg");

    SmileText->Bind(GL_TEXTURE0);
    WoodText->Bind(GL_TEXTURE1);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); //default: GL_LESS

    //camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60;
    float AspectRatio = 600.f / 400.f;
    float ZNear = 0.01f;
    float ZFar = 1000.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);

    
    glm::vec3 BasePos{-5.f, 0, 0};

    Quads.resize(3);
    for(int Index = 0; Index < Quads.size(); Index++)
    {
        Quads[Index].Position = BasePos;
        Quads[Index].Rotation = Index * 10.f;
       // Quads[Index].Scale = glm::vec3(1.f + Index * 0.5f);
        Quads[Index].Scale = {1,1,1};
        BasePos += glm::vec3(1.5, 0.f, 0.f);
    }

    glGenBuffers(1, &VboMvp);
    glBindBuffer(GL_ARRAY_BUFFER, VboMvp);
    glBufferData(GL_ARRAY_BUFFER, Quads.size() * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);

    GLuint Location_Mvp = 2;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    MvpData.resize(Quads.size());
}

void Ex09InstancingDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Example with 1 drawcall for each instance
    for(int Index = 0; Index < 8; Index++)
    {
        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, Quads[Index].Position);

        glm::mat4 Mvp = Projection * View * Model;
        Program->SetUniform("mvp", Mvp);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    }
    */

    //Example using with gl_InstanceID in vertex shader for traslation purpose
    //glm::mat4 Mvp = Projection * View * glm::mat4(1.f);
    //Program->SetUniform("mvp", Mvp);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;  

    float BaseAngle = ElapsedTime * 20.f;

    for(int Index = 0; Index < MvpData.size(); Index++)
    {
        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, Quads[Index].Position);
        Model = glm::rotate(Model, glm::radians(BaseAngle + Quads[Index].Rotation), glm::vec3(0, 0, 1));
        Model = glm::scale(Model, Quads[Index].Scale);

        glm::mat4 Mvp = Projection * View * Model;

        MvpData[Index] = Mvp;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, MvpData.size() * sizeof(glm::mat4), MvpData.data());

    //glDrawArraysInstanced()
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 8);
}

void Ex09InstancingDraw::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &Ebo);
    delete Program;
    delete SmileText;
    delete WoodText;
}