#include "Ex10Exercise.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>
//#define OBJ_PARSER_IMPLEMENTATION
#include "obj-parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Ex10Exercise::Start()
{
    //Storm Trupper scenario
    Program = new OGLProgram("resources/shaders/exercise.vert", "resources/shaders/exercise.frag");

    obj_t* mesh = obj_parser_parse("resources/models/stormtrooper.obj");

    // storm trooper 
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


    PointLightPos = glm::vec3(1, 0, 0);
    Program->Bind();
    Program->SetUniform("point_light_pos", PointLightPos);
    Program->SetUniform("camera_pos", Position);
    StormText = new OGLTexture("resources/models/stormtrooper.png");
    StormText->Bind(GL_TEXTURE0);


    //Light scenario
    CubeProgram = new OGLProgram("resources/shaders/exercise_light.vert", "resources/shaders/exercise_light.frag");
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

    //1. Create VAO
    glGenVertexArrays(1, &CubeVao);
    glBindVertexArray(CubeVao);
    
    //2. Create VBO to load data
    glGenBuffers(1, &CubeVbo);
    glBindBuffer(GL_ARRAY_BUFFER, CubeVbo);

    int CubeDataSize = CubeVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, CubeDataSize, CubeVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    CubeProgram->Bind();
    CubeProgram->SetUniform("color", Color{1,1,1,1});
}

void Ex10Exercise::Update(float InDeltaTime)
{
    glm::vec3 StormTrupperPos = glm::vec3(3, -4, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 20.f * ElapsedTime;

    Program->Bind();
    glBindVertexArray(Vao);

    //Model =  translate * rotate * scale;
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, StormTrupperPos);
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));


    glm::mat4 Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

    //cubes
    CubeProgram->Bind();
    glBindVertexArray(CubeVao);
 
    glm::mat4 CubeModel = glm::mat4(1.f);
    CubeModel = glm::translate(CubeModel, PointLightPos + StormTrupperPos + glm::vec3(0,4,0)); //vec3(1, 0, 0) + vec3(3, -4, 0) + pivot cancelling
    CubeModel = glm::scale(CubeModel, glm::vec3(0.2f));

    glm::mat4 T1 = glm::translate(glm::mat4(1.f), StormTrupperPos);
    glm::mat4 Rotation = glm::rotate(glm::mat4(1.f), glm::radians(Angle), glm::vec3(0, 1, 0));
    glm::mat4 T2 = glm::translate(glm::mat4(1.f), -StormTrupperPos);
    glm::mat4 RotationAround = T1 * Rotation * T2;


    glm::mat4 CubeMvp = Projection * View * RotationAround * CubeModel;
    CubeProgram->SetUniform("mvp", CubeMvp);
    glDrawArrays(GL_TRIANGLES, 0, CubeVerticeCount);

}

void Ex10Exercise::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteVertexArrays(1, &CubeVao);
    glDeleteBuffers(1, &Vbo);
    glDeleteBuffers(1, &CubeVbo);
    delete Program;
    delete CubeProgram;
    delete StormText;
}