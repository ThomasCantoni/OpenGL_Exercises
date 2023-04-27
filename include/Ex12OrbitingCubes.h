#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <vector>


struct CubeData
{ 
    glm::vec3 Position;
    glm::vec3 Rotation;
    glm::vec3 Scale;
};

class Ex12OrbitingCubes 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private: 
    glm::mat4 CameraView;
    glm::mat4 CameraProjection;
    glm::vec3 PointLightPos;

    //stormtrooper
    OGLProgram* StormProgram;
    glm::vec3 StormTrupperPos;
    int VerticeCount;
    GLuint StormVao;
    GLuint StormVbo;
    OGLTexture* StormText;
    
private: //cubes
    OGLProgram* CubeProgram;
    int CubeVerticeCount,CubeCount;
    GLuint CubesVao;
    GLuint CubesVbo;
    GLuint CubesVboMvp;
    std::vector<CubeData> CubesTransforms;
    std::vector<uint32_t> CubeIndices;
    std::vector<glm::mat4> MvpData;
};
