#include "Ex11PostFX.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>


void Ex11PostFX::Start()
{
    Program = new OGLProgram("resources/shaders/postfx_scene.vert", "resources/shaders/postfx_scene.frag");
    
    std::vector<float> Vertices = {
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

    //4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    //Program->Bind();

    WoodText = new OGLTexture("resources/textures/wood-box.jpg");
    //WoodText->Bind(GL_TEXTURE0);

    /*
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); //default
    glCullFace(GL_BACK); //default
    */

    /* POSTFX: PREPARE FRAMEBUFFER for drawing "off-screen" */
    glGenFramebuffers(1, &SceneFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);

    //1. Attach Color to framebuffer
    glGenTextures(1, &SceneTexture);
    glBindTexture(GL_TEXTURE_2D, SceneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 400, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Wrapping
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SceneTexture, 0);

    //2. Attach depth to framebuffer
    glGenRenderbuffers(1, &SceneRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, SceneRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 600, 400);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, SceneRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind current render buffer.

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
    {
        DIE("Framebuffer not ready!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind and back to SwapChain framebuffer

    /* POST FX PIPELINE */
    QuadProgram = new OGLProgram("resources/shaders/postfx_quad.vert", "resources/shaders/postfx_quad.frag");

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
}

void Ex11PostFX::Update(float InDeltaTime)
{
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    
    //DRAW SCENE
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray(Vao);
    Program->Bind();
    //Program->SetUniform("rotation", 20.f * ElapsedTime);
    WoodText->Bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //DRAW QUAD (Apply FX)
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //back to Swapchain as output
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(QuadVao);
    QuadProgram->Bind();
    QuadProgram->SetUniform("time", ElapsedTime);
    //glTextureBin
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, SceneTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Ex11PostFX::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
    delete WoodText;

    glDeleteFramebuffers(1, &SceneFbo);
    glDeleteTextures(1, &SceneTexture);
    glDeleteRenderbuffers(1, &SceneRbo);

    glDeleteVertexArrays(1, &QuadVao);
    glDeleteBuffers(1, &QuadVbo);
    delete QuadProgram;
}