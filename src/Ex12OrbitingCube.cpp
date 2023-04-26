#include "Ex12OrbitingCubes.h"
#include "Common.h" 
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <vector>
//#define OBJ_PARSER_IMPLEMENTATION
#include "obj-parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Ex12OrbitingCubes::Start()
{
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


    PointLightPos = glm::vec3(1, 0, 0);


    //Stormtrooper scenario
    StormProgram = new OGLProgram("resources/shaders/exercise.vert", "resources/shaders/exercise.frag");

    obj_t* mesh = obj_parser_parse("resources/models/stormtrooper.obj");

    // storm trooper mesh
    std::vector<float> Vertices;
    for(int i=0; i < mesh->face_count; ++i) 
    {
        obj_triangle_t& t = mesh->triangles[i];
        
        Vertices.push_back(t.v1.position.x);
        Vertices.push_back(t.v1.position.y);
        Vertices.push_back(t.v1.position.z);
        Vertices.push_back(t.v1.uv.x);
        Vertices.push_back(t.v1.uv.y);
        Vertices.push_back(t.v1.normal.x);
        Vertices.push_back(t.v1.normal.y);
        Vertices.push_back(t.v1.normal.z);

        Vertices.push_back(t.v2.position.x);
        Vertices.push_back(t.v2.position.y);
        Vertices.push_back(t.v2.position.z);
        Vertices.push_back(t.v2.uv.x);
        Vertices.push_back(t.v2.uv.y);
        Vertices.push_back(t.v2.normal.x);
        Vertices.push_back(t.v2.normal.y);
        Vertices.push_back(t.v2.normal.z);

        Vertices.push_back(t.v3.position.x);
        Vertices.push_back(t.v3.position.y);
        Vertices.push_back(t.v3.position.z);
        Vertices.push_back(t.v3.uv.x);
        Vertices.push_back(t.v3.uv.y);
        Vertices.push_back(t.v3.normal.x);
        Vertices.push_back(t.v3.normal.y);
        Vertices.push_back(t.v3.normal.z);
    }
    VerticeCount = Vertices.size() / 8;

    //1. Create stormtrooper VAO
    glGenVertexArrays(1, &StormVao);
    glBindVertexArray(StormVao);

    //2. Create stromtrooper VBO to load data
    glGenBuffers(1, &StormVbo);
    glBindBuffer(GL_ARRAY_BUFFER, StormVbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    //4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW); //default
    //glCullFace(GL_BACK); //default

    StormProgram->Bind();
    StormProgram->SetUniform("point_light_pos", PointLightPos);
    StormProgram->SetUniform("camera_pos", Position);
    StormText = new OGLTexture("resources/models/stormtrooper.png");
    StormText->Bind(GL_TEXTURE0);

    // CUBES NOW
    
    //cubes require their own program for instancing
    CubeProgram = new OGLProgram("resources/shaders/orbit.vert", "resources/shaders/orbit.frag");

    //define the mesh of a single cube
    std::vector<float> CubeVertices = {
        //FRONT FACE
        //positions      uvs
        -1, -1, 1,   0, 0,    //bottom-left
         1, -1, 1,   1, 0,    //bottom-right
         1,  1, 1,   1, 1,    //top-right
        -1,  1, 1,    0, 1,    //top-left 
        -1, -1, 1,   0, 0,    //bottom-left
         1,  1, 1,   1, 1,    //top-right

        //BACK FACE
         1, -1,-1,   0, 0,    //bottom-left
        -1, -1,-1,   1, 0,    //bottom-right
        -1,  1,-1,   1, 1,    //top-right
         1,  1,-1,   0, 1,    //top-left 
         1, -1,-1,   0, 0,    //bottom-left
        -1,  1,-1,   1, 1,    //top-right

         //LEFT FACE
        -1, -1,-1,   0, 0,    //bottom-left
        -1, -1, 1,   1, 0,    //bottom-right
        -1,  1, 1,   1, 1,    //top-right
        -1,  1,-1,   0, 1,    //top-left 
        -1, -1,-1,   0, 0,    //bottom-left
        -1,  1, 1,   1, 1,    //top-right

        //RIGHT FACE
         1, -1, 1,   0, 0,    //bottom-left
         1, -1,-1,   1, 0,    //bottom-right
         1,  1,-1,   1, 1,    //top-right
         1,  1, 1,   0, 1,    //top-left 
         1, -1, 1,   0, 0,    //bottom-left
         1,  1,-1,   1, 1,    //top-right

         //TOP FACE
         -1, 1, 1,   0, 0,    //bottom-left
          1, 1, 1,   1, 0,    //bottom-right
          1, 1,-1,   1, 1,    //top-right
         -1, 1,-1,   0, 1,    //top-left 
         -1, 1, 1,   0, 0,    //bottom-left
          1, 1,-1,   1, 1,    //top-right
         
         //BOTTOM FACE
         -1,-1,-1,   0, 0,    //bottom-left
          1,-1,-1,   1, 0,    //bottom-right
          1,-1, 1,   1, 1,    //top-right
         -1,-1, 1,   0, 1,    //top-left 
         -1,-1,-1,   0, 0,    //bottom-left
          1,-1, 1,   1, 1,    //top-right
    };
    CubeVerticeCount = CubeVertices.size() / 5;

    CubeCount = 1;
    glm::vec3 BasePos = {-3,0,0};
    Cubes.resize(CubeCount);
    for(int Index = 0; Index <CubeCount; Index++)
    {
        Cubes[Index].Position = BasePos;
        Cubes[Index].Rotation = glm::vec3(0);
        //Quads[Index].Scale = glm::vec3(1.f + Index * 0.5f);
        Cubes[Index].Scale = glm::vec3(1);
        BasePos += glm::vec3(0.5f, 0.f, 0.f);
    }
    glGenVertexArrays(1, &CubesVao);
    glBindVertexArray(CubesVao);
    
   
    glGenBuffers(1, &CubesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, CubesVbo);

    int CubeDataSize = CubeVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, CubeDataSize, CubeVertices.data(), GL_STREAM_DRAW);

    //3. Set current buffer to have these settings
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1,2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)3);
    //glEnableVertexAttribArray(1);
//
    
   //glGenBuffers(1, &CubesEbo);
   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubesEbo);
   //int EboSize = Indexes.size() * sizeof(uint32_t);
   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, EboSize, Indexes.data(), GL_STATIC_DRAW);
    CubeProgram->Bind();
    CubeProgram->SetUniform("color", Color{1,0,0,1});

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

    MvpData.resize(Cubes.size());
   
}
void Ex12OrbitingCubes::Update(float InDeltaTime)
{
    glm::vec3 StormTrupperPos = glm::vec3(3, -4, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 20.f * ElapsedTime;


    //Model =  translate * rotate * scale;
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, StormTrupperPos);
   // Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    
    glm::mat4 Mvp = CameraProjection * CameraView * Model;
    StormProgram->Bind();
    glBindVertexArray(StormVao);
    StormProgram->SetUniform("mvp", Mvp);
    StormProgram->SetUniform("model", Model);
   
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);


 
    
    
    
  //glm::mat4 T1 = glm::translate(glm::mat4(1.f), StormTrupperPos);
  //glm::mat4 Rotation = glm::rotate(glm::mat4(1.f), glm::radians(Angle), glm::vec3(0, 1, 0));
  //glm::mat4 T2 = glm::translate(glm::mat4(1.f), -StormTrupperPos);
  //glm::mat4 RotationAround = T1 * Rotation * T2;

  
    //glm::mat4 CubeModel = glm::mat4(1.f);
    //CubeModel = glm::translate(CubeModel, {2,2.8f,0}); //vec3(1, 0, 0) + vec3(3, -4, 0) + pivot cancelling
    //CubeModel = glm::scale(CubeModel, glm::vec3(0.2f));
    //glm::mat4 CubeMvp = CameraProjection * CameraView  *RotationAround* CubeModel;
    for(int Index = 0; Index < MvpData.size(); Index++)
    {
        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, Cubes[Index].Position);
        Model = glm::rotate(Model, glm::radians(ElapsedTime*50.0f), glm::vec3(0, 1, 0));
        Model = glm::scale(Model, Cubes[Index].Scale);

        glm::mat4 Mvp = CameraProjection * CameraView * Model;

        MvpData[Index] = Mvp;
        //CubeProgram->SetUniform("mvp", CubeMvp);
    }
    
    
    CubeProgram->Bind();
    glBindVertexArray(CubesVao);
    glBufferSubData(GL_ARRAY_BUFFER, 0, MvpData.size() * sizeof(glm::mat4), MvpData.data());

 
    glDrawArraysInstanced(GL_TRIANGLES,0,CubeVerticeCount,CubeCount);
    std::cout<<CubeVerticeCount<<std::endl;
    //glBufferData(GL_ARRAY_BUFFER, CubeCount * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    //glDrawArraysInstanced(GL_TRIANGLES,0,12,3);
    //glDrawElementsInstanced(GL_TRIANGLES,)
   
    //currentCubePos = glm::vec3(0,sin(ElapsedTime)*5.f,0);
   

}

void Ex12OrbitingCubes::Destroy()
{
    glDeleteVertexArrays(1, &StormVao);
    glDeleteVertexArrays(1, &CubesVao);
    glDeleteBuffers(1, &StormVbo);
    glDeleteBuffers(1, &CubesVbo);
    delete StormProgram;
    delete CubeProgram;
    delete StormText;
}