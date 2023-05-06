#pragma once 
#include "glad/glad.h"

struct light_t_cpu_side
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec3 spec_color;
    float spec_mul;
};