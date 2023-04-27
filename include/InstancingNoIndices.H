#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "OGLProgram.h"
#include "OGLTexture.h"
#include "CubeData.h"




class InstancingNoIndices
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

    GLuint CubeVbo;
    GLuint CubeVboMvp;
    cubedata_t Cube;
    std::vector<cubedata_t> CubesTransforms;
    std::vector<glm::mat4> MvpData;
};
