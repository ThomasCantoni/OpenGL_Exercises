#include "Ex12DeferredDraw.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>
#include "obj-parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Ex12DeferredDraw::Start()
{
    Program = new OGLProgram("resources/shaders/deferred_scene.vert", "resources/shaders/deferred_scene.frag");

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


    //glm::vec3 PointLightPos = glm::vec3(1, 0, 0);
    //Program->SetUniform("point_light_pos", PointLightPos);
    //Program->SetUniform("camera_pos", Position);

    /* SETUP G-BUFFER (MRT - Multi Render Target) (G-Buffer alias Geometry Buffer) */
    //1. Create FBO
    glGenFramebuffers(1, &GFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, GFbo);

    //2. Create Render Target (as color attach): Diffuse (x3 float => with 8bit precision)
    glGenTextures(1, &DiffuseText);
    glBindTexture(GL_TEXTURE_2D, DiffuseText);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 400, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, DiffuseText, 0);

    //3. Create Render Target (as color attach): Normals (x3 float => with 16bit precision )
    glGenTextures(1, &NormalText);
    glBindTexture(GL_TEXTURE_2D, NormalText);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 600, 400, 0, GL_RGB, GL_FLOAT, NULL); //TODO: Try with GL_RGB16F 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalText, 0);

    //4. Create Render Target (as color attach): Positions (x3 float => with 16bit precision )
    glGenTextures(1, &PositionText);
    glBindTexture(GL_TEXTURE_2D, PositionText);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 600, 400, 0, GL_RGB, GL_FLOAT, NULL); //TODO: Try with GL_RGB16F 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, PositionText, 0);

    //5. Make available the Render Targets
    GLenum Attachs[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, Attachs);

    //6. Attach Depth RenderBuffer
    glGenRenderbuffers(1, &DepthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, DepthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 600, 400);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); 

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) DIE("FBO not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /* SETUP Blending */
    QuadProgram = new OGLProgram("resources/shaders/deferred_quad.vert", "resources/shaders/deferred_quad.frag");

    std::vector<float> QuadVertices = { //NDC to avoid trasformations in vert shader
        // Positions   // Uvs
        -1.f, -1.f,  0.f, 0.f,  //bottom left
         1.f, -1.f,  1.f, 0.f,  //bottom right
        -1.f,  1.f,  0.f, 1.f,  //top left

        -1.f,  1.f,  0.f, 1.f,  //top left
         1.f, -1.f,  1.f, 0.f,  //bottom right
         1.f,  1.f,  1.f, 1.f   //top right    
    };

    //1. Create VAO
    glGenVertexArrays(1, &QuadVao);
    glBindVertexArray(QuadVao);

    //2. Create VBO to load data
    glGenBuffers(1, &QuadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVbo);

    glBufferData(GL_ARRAY_BUFFER, QuadVertices.size() * sizeof(float), QuadVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glm::vec3 PointLightPos0 = glm::vec3(1, 0, 0);
    glm::vec3 PointLightPos1 = glm::vec3(-2, 0, 0);
    glm::vec3 PointLightPos2 = glm::vec3(0, 4, 0);

    QuadProgram->Bind();
    QuadProgram->SetUniform("point_light_pos[0]", PointLightPos0);
    QuadProgram->SetUniform("point_light_pos[1]", PointLightPos1);
    QuadProgram->SetUniform("point_light_pos[2]", PointLightPos2);
    QuadProgram->SetUniform("camera_pos", Position);

     //QuadProgram->SetUniform("lights[0].pos", PointLightPos0);
}

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

void Ex12DeferredDraw::Update(float InDeltaTime)
{
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 20.f * ElapsedTime;

    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));
    glm::mat4 Mvp = Projection * View * Model;

    // Geometry Pass
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GFbo); //GL_FRAMEBUFFER = Read + Write
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

    //Debug
    //DebugGBuffer(GFbo);

    // Blending Pass
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, GFbo);

    QuadProgram->Bind();
    glBindVertexArray(QuadVao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DiffuseText); 

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, NormalText); 

    //glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D, PositionText); 
    glBindTextureUnit(2, PositionText); //Alternative "compact" API 

    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void Ex12DeferredDraw::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
    delete StormText;

    glDeleteFramebuffers(1, &GFbo);
    glDeleteTextures(1, &DiffuseText);
    glDeleteTextures(1, &NormalText);
    glDeleteTextures(1, &PositionText);
    glDeleteRenderbuffers(1, &DepthRbo);

    delete QuadProgram;
    glDeleteVertexArrays(1, &QuadVao);
    glDeleteBuffers(1, &QuadVbo);
}