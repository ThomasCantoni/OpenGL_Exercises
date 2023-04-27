#include "InstancingNoIndices.h"
#include "Common.h" 
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <vector>
//#define OBJ_PARSER_IMPLEMENTATION
#include "obj-parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


void InstancingNoIndices::Start()
{
   
    //4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    //camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60;
    float AspectRatio = 600.f / 400.f;
    float ZNear = 0.01f;
    float ZFar = 1000.f;

    CameraView = glm::lookAt(Position, Position + Direction, Up);
    CameraProjection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);


    
    //cubes require their own program for instancing
    CubeProgram = new OGLProgram("resources/shaders/orbit.vert", "resources/shaders/orbit.frag");

    //define the mesh of a single cube
    std::vector<float> IndexedCubeVertices = {
        //FRONT FACE
        //positions      
        -1, -1, 1,       //bottom-left
         1, -1, 1,       //bottom-right
         1,  1, 1,       //top-right
        -1,  1, 1,       //top-left 
        -1, -1, 1,       //bottom-left
         1,  1, 1,       //top-right

        //BACK FACE
         1, -1,-1,       //bottom-left
        -1, -1,-1,       //bottom-right
        -1,  1,-1,       //top-right
         1,  1,-1,       //top-left 
         1, -1,-1,       //bottom-left
        -1,  1,-1,       //top-right

         //LEFT FACE
        -1, -1,-1,       //bottom-left
        -1, -1, 1,       //bottom-right
        -1,  1, 1,       //top-right
        -1,  1,-1,       //top-left 
        -1, -1,-1,       //bottom-left
        -1,  1, 1,       //top-right

        //RIGHT FACE
         1, -1, 1,       //bottom-left
         1, -1,-1,       //bottom-right
         1,  1,-1,       //top-right
         1,  1, 1,       //top-left 
         1, -1, 1,       //bottom-left
         1,  1,-1,       //top-right

         //TOP FACE
         -1, 1, 1,       //bottom-left
          1, 1, 1,       //bottom-right
          1, 1,-1,       //top-right
         -1, 1,-1,       //top-left 
         -1, 1, 1,       //bottom-left
          1, 1,-1,       //top-right
         
         //BOTTOM FACE
         -1,-1,-1,      //bottom-left
          1,-1,-1,      //bottom-right
          1,-1, 1,      //top-right
         -1,-1, 1,      //top-left 
         -1,-1,-1,      //bottom-left
          1,-1, 1,      //top-right
    };
    CubeVerticeCount = IndexedCubeVertices.size()/3;

   
 

    CubeCount = 3;
    MvpData.resize(CubeCount);
    CubesTransforms.resize(CubeCount);
    glm::vec3 BasePos = {-4,-1,0};
    for (int i = 0; i < CubeCount; i++)
    {
      
        CubesTransforms[i].Position = BasePos;
        CubesTransforms[i].Rotation = glm::vec3(0);
        CubesTransforms[i].Scale = glm::vec3(1);
            BasePos += glm::vec3(2,1,0);
        
        std::cout<<
        CubesTransforms[i].Position.x<<" "<<
        CubesTransforms[i].Position.y<<" "<<
        CubesTransforms[i].Position.z<<std::endl;
        
    }
    CubeProgram->Bind();
    glGenVertexArrays(1, &CubeVao);
    glBindVertexArray(CubeVao);
    
    glGenBuffers(1, &CubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVbo);
    int CubeDataSize = IndexedCubeVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, CubeDataSize, IndexedCubeVertices.data(), GL_STATIC_DRAW);

    //3. link the buffer we just created to the vertex shader
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glGenBuffers(1,&CubeVboMvp);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVboMvp);
    glBufferData(GL_ARRAY_BUFFER, CubeCount * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    


    GLuint Location_Mvp = 3;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(0* sizeof(glm::vec4)));
    glEnableVertexAttribArray(Location_Mvp);
    glVertexAttribDivisor(Location_Mvp, 1);

    Location_Mvp++;
    glVertexAttribPointer(Location_Mvp, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1* sizeof(glm::vec4)));
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
    CubeProgram->SetUniform("color", Color{0 ,1, 0, 1});
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); //default: GL_LESS
    
    //glEnable(GL_CULL_FACE);
   
}
void InstancingNoIndices::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 20.f * ElapsedTime;

   
    for (int i = 0; i < CubeCount; i++)
    {
       
        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, CubesTransforms[i].Position);
        Model = glm::rotate(Model, glm::radians(ElapsedTime*50.0f), glm::vec3(0, 1, 0));
        //Model = glm::scale(Model, CubesTransforms[i].Scale);
        glm::mat4 Mvp = CameraProjection * CameraView * Model;

        MvpData[i] = Mvp;
    }
 
    CubeProgram->Bind();
    glBindVertexArray(CubeVao);
   
    glBufferSubData(GL_ARRAY_BUFFER, 0, MvpData.size() * sizeof(glm::mat4), MvpData.data());
   
    glDrawArraysInstanced(GL_TRIANGLES,0,CubeVerticeCount,CubeCount);

}

void InstancingNoIndices::Destroy()
{

    glDeleteVertexArrays(1, &CubeVao);
 
    glDeleteBuffers(1, &CubeVbo);

    delete CubeProgram;

}