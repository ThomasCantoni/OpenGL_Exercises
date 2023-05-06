#include "Ex14ManyLightsFORWARD.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>
#include "obj-parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <format>

void Ex14ManyLightsFORWARD::Start()
{
    Program = new OGLProgram("resources/shaders/phong.vert", "resources/shaders/many_lights_FORWARD.frag");

    obj_t* mesh = obj_parser_parse("resources/models/stormtrooper.obj");

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
    //glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    //Program->Bind();

    StormText = new OGLTexture("resources/models/stormtrooper.png");
    

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
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


    

   
    Program->Bind();
    //QUAD PROGRAM light_t INIT
    int light_number = 200;
    Program->SetUniform("light_num",light_number);
    lights_struct.resize(light_number);
    int y_factor=0;
    for(int i = 0; i<light_number;++i)
    {

        if(i % 10 > 0)
        {
            ++y_factor;
        }
        lights_struct[i].pos = glm::vec3(-2,4,0);
        lights_struct[i].color = glm::vec3(1,1,1);
        lights_struct[i].spec_color = glm::vec3(1,1,1);
        lights_struct[i].spec_mul = 1;
    }

    
    for (int i = 0; i < light_number; ++i)
    {

    std::string string_passer = std::format("lights[{}]",i);
    
    Program->SetUniform(string_passer+  ".pos",lights_struct[i].pos);
    

    Program->SetUniform(string_passer+ ".color",lights_struct[i].color);
   

    Program->SetUniform(string_passer+ ".specular_color",lights_struct[i].spec_color);
   

    Program->SetUniform(string_passer+ ".specular_factor",lights_struct[i].spec_mul);
   
    }
   
   
}
/*
static void DebugGBuffer(GLuint GBufferId) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);


    glBindFramebuffer(GL_READ_FRAMEBUFFER, GBufferId);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(
        0, 0,   600, 400, //Source Bounds
        0, 200, 300, 400, //Destin Bounds
        GL_COLOR_BUFFER_BIT,  //Which buffer to write to
        GL_LINEAR        //interpolation in case of streching image (LINEAR, NEAREST)
    );

    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(
        0, 0,     600, 400, //Source Bounds
        300, 200, 600, 400, //Destin Bounds
        GL_COLOR_BUFFER_BIT,  //Which buffer to write to
        GL_LINEAR        //interpolation in case of streching image (LINEAR, NEAREST)
    );

    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(
        0, 0,     600, 400, //Source Bounds
        0, 0,     300, 200, //Destin Bounds
        GL_COLOR_BUFFER_BIT,  //Which buffer to write to
        GL_LINEAR        //interpolation in case of streching image (LINEAR, NEAREST)
    );
}
*/
void Ex14ManyLightsFORWARD::Update(float InDeltaTime)
{
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 20.f * ElapsedTime;

    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));
    glm::mat4 Mvp = Projection * View * Model;


   
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, 600, 400);

    glBindVertexArray(Vao);
    Program->Bind();
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);
    StormText->Bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

    return;
    Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(2, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));
    Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

    Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(-4, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));
    Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

     Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(-2, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));
    Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(4, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));
    Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);


}

void Ex14ManyLightsFORWARD::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
    delete StormText;

}