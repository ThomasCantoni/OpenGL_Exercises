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
     StormTrupperPos = glm::vec3(3, -4, 0);
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
    obj_t* Suzanne = obj_parser_parse("resources/models/suzanne.obj");

    //define the mesh of a single cube
     std::vector<float> IndexedCubeVertices;
     IndexedCubeVertices.resize(Suzanne->face_count);
    for(int i=0; i < Suzanne->face_count; ++i) 
    {
        obj_triangle_t& t = Suzanne->triangles[i];
        
        IndexedCubeVertices.push_back(t.v1.position.x);
        IndexedCubeVertices.push_back(t.v1.position.y);
        IndexedCubeVertices.push_back(t.v1.position.z);
        IndexedCubeVertices.push_back(t.v1.uv.x);
        IndexedCubeVertices.push_back(t.v1.uv.y);
        IndexedCubeVertices.push_back(t.v1.normal.x);
        IndexedCubeVertices.push_back(t.v1.normal.y);
        IndexedCubeVertices.push_back(t.v1.normal.z);

        IndexedCubeVertices.push_back(t.v2.position.x);
        IndexedCubeVertices.push_back(t.v2.position.y);
        IndexedCubeVertices.push_back(t.v2.position.z);
        IndexedCubeVertices.push_back(t.v2.uv.x);
        IndexedCubeVertices.push_back(t.v2.uv.y);
        IndexedCubeVertices.push_back(t.v2.normal.x);
        IndexedCubeVertices.push_back(t.v2.normal.y);
        IndexedCubeVertices.push_back(t.v2.normal.z);

        IndexedCubeVertices.push_back(t.v3.position.x);
        IndexedCubeVertices.push_back(t.v3.position.y);
        IndexedCubeVertices.push_back(t.v3.position.z);
        IndexedCubeVertices.push_back(t.v3.uv.x);
        IndexedCubeVertices.push_back(t.v3.uv.y);
        IndexedCubeVertices.push_back(t.v3.normal.x);
        IndexedCubeVertices.push_back(t.v3.normal.y);
        IndexedCubeVertices.push_back(t.v3.normal.z);
    }
     
    //     //FRONT FACE
    //     //positions      
    //     -1, -1, 1,       //bottom-left
    //      1, -1, 1,       //bottom-right
    //      1,  1, 1,       //top-right
    //     -1,  1, 1,       //top-left 
    //     -1, -1, 1,       //bottom-left
    //      1,  1, 1,       //top-right

    //     //BACK FACE
    //      1, -1,-1,       //bottom-left
    //     -1, -1,-1,       //bottom-right
    //     -1,  1,-1,       //top-right
    //      1,  1,-1,       //top-left 
    //      1, -1,-1,       //bottom-left
    //     -1,  1,-1,       //top-right

    //      //LEFT FACE
    //     -1, -1,-1,       //bottom-left
    //     -1, -1, 1,       //bottom-right
    //     -1,  1, 1,       //top-right
    //     -1,  1,-1,       //top-left 
    //     -1, -1,-1,       //bottom-left
    //     -1,  1, 1,       //top-right

    //     //RIGHT FACE
    //      1, -1, 1,       //bottom-left
    //      1, -1,-1,       //bottom-right
    //      1,  1,-1,       //top-right
    //      1,  1, 1,       //top-left 
    //      1, -1, 1,       //bottom-left
    //      1,  1,-1,       //top-right

    //      //TOP FACE
    //      -1, 1, 1,       //bottom-left
    //       1, 1, 1,       //bottom-right
    //       1, 1,-1,       //top-right
    //      -1, 1,-1,       //top-left 
    //      -1, 1, 1,       //bottom-left
    //       1, 1,-1,       //top-right
         
    //      //BOTTOM FACE
    //      -1,-1,-1,      //bottom-left
    //       1,-1,-1,      //bottom-right
    //       1,-1, 1,      //top-right
    //      -1,-1, 1,      //top-left 
    //      -1,-1,-1,      //bottom-left
    //       1,-1, 1,      //top-right
    // };
    
    CubeVerticeCount = IndexedCubeVertices.size() / 8;

   
    CubeCount = 4;
    glm::vec3 BasePos[] ={

     StormTrupperPos +glm::vec3(1.5f,   6.75f,0),
     StormTrupperPos +glm::vec3(-1.5f,  6.75f,0),
     StormTrupperPos +glm::vec3(0,      6.75f,1.5f),
     StormTrupperPos +glm::vec3(0,      6.75f,-1.5f)

     };

     MvpData.resize(CubeCount);
    CubesTransforms.resize(CubeCount);
    for(int i = 0; i <CubeCount; ++i)
    {
        CubesTransforms[i].Position = BasePos[i];
        CubesTransforms[i].Rotation = glm::vec3(0);
        CubesTransforms[i].Scale = glm::vec3(0.2f);
        
    }

    CubeProgram->Bind();
    glGenVertexArrays(1, &CubesVao);
    glBindVertexArray(CubesVao);
    
    glGenBuffers(1, &CubesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, CubesVbo);
    int CubeDataSize = IndexedCubeVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, CubeDataSize, IndexedCubeVertices.data(), GL_STREAM_DRAW);

    //3. Set current buffer to have these settings
    glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    //this buffer is going to store the information for each instance
    glGenBuffers(1,&CubesVboMvp);
    glBindBuffer(GL_ARRAY_BUFFER, CubesVboMvp);
    glBufferData(GL_ARRAY_BUFFER, CubeCount * sizeof(glm::mat4), NULL, GL_STREAM_DRAW);
    
  

    //the idea of instancing stems from the fact that on the gpu
    // we have the definition of a single mesh that gets multiplied with a translation,rotation and scale matrices
    // and that means we can use the same mesh but only change the matrices in order to create
    // virtual copies of the same object without having to redefine the mesh multiple times.
    // This saves gpu memory

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL); //default: GL_LESS
    CubeProgram->SetUniform("color", Color{1,1,0,1});
  
   
}
void Ex12OrbitingCubes::Update(float InDeltaTime)
{
    

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 50.f * ElapsedTime;


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

    //CUBES
  
    float deltaAngle = 360.f / (float)CubeCount;
    for(int i = 0; i < MvpData.size(); ++i)
    {
        float offsetAngle = deltaAngle * i;
        float x = cos(glm::radians(offsetAngle));
        float z = sin(glm::radians(offsetAngle));
        glm::vec3 dir = glm::vec3(x,0,z);

        glm::mat4 Model = glm::mat4(1.f);
        Model = glm::translate(Model, CubesTransforms[i].Position);
        glm::mat4 T1 = glm::translate(glm::mat4(1.f), StormTrupperPos);
        glm::mat4 Rotation = glm::rotate(glm::mat4(1.f), glm::radians(Angle), glm::vec3(0, 1, 0));
        glm::mat4 T2 = glm::translate(glm::mat4(1.f), -StormTrupperPos);
        glm::mat4 RotationAround = T1 * Rotation * T2;

        //Only if to take account for suzanne that look in the opposite direction of the screen
        //take this into account only if suzanne is looking at the opposite direction of the camera
        Model= glm::rotate(Model, glm::radians(180.f), glm::vec3(0,1,0)); 
        glm::mat4 lookAtResult = glm::lookAt(glm::vec3(0), dir, glm::vec3(0,1,0));
        Model *= glm::inverse(lookAtResult);
     
             
        Model = glm::rotate(Model, glm::radians(ElapsedTime*50.0f), glm::vec3(0, 1, 0));
        Model = glm::scale(Model, CubesTransforms[i].Scale);

        glm::mat4 Mvp = CameraProjection * CameraView *RotationAround * Model;
        MvpData[i] = Mvp;

    }
    CubeProgram->Bind();
    glBindVertexArray(CubesVao);
    
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, MvpData.size() * sizeof(glm::mat4), MvpData.data());
    
 
    glDrawArraysInstanced(GL_TRIANGLES,0,CubeVerticeCount,CubeCount);
   
   

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