#pragma once
#include <glad/glad.h>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <vector>
#include "CubeData.h"




class InstancingWithIndices
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private: 
    glm::mat4 CameraView;
    glm::mat4 CameraProjection;
    glm::vec3 PointLightPos;

   
private: //cubes
    OGLProgram* CubeProgram;
    int CubeVerticeCount,CubeCount;
    GLuint CubeVao;
    GLuint CubeEbo;
    GLuint CubeVbo;
    GLuint CubeVboMvp;
    cubedata_t Cube;
    std::vector<uint32_t> CubeIndices;
    std::vector<cubedata_t> CubesTransforms;
    std::vector<glm::mat4> MvpData;
};
