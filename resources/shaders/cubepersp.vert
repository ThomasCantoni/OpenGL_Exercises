#version 450 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;

out vec2 vert_uv_out;

uniform float rotation;
 
vec3 perspective(vec3 pos)
{
    float aspect_ratio = 600.f / 400.f;
    float vert_fov = 60.f;
    float half_fov = vert_fov * 0.5f;
    float fov = tan(radians(half_fov));

    float near = 0.01f;
    float far = 1000.f;
    float range = far - near;

    vec3 persp;
    persp.x = pos.x / -pos.z;
    persp.y = pos.y / -pos.z;
    persp.z = ( (-pos.z - near) / range )  * 2.f - 1.f;
    
    persp.x = persp.x / ( fov * aspect_ratio);
    persp.y = persp.y / fov;
    
    return persp;
}

vec3 rotate_on_y(vec3 pos, float degrees) 
{
    float rads = radians(degrees);
    vec3 result = pos;
    result.x = cos(rads) * pos.x - sin(rads) * pos.z;
    result.z = sin(rads) * pos.x + cos(rads) * pos.z;
    return result;
}

void main()
{
    vec3 fixed_vert = vert_pos;
    fixed_vert = rotate_on_y(fixed_vert, rotation);
    fixed_vert.z -= 4.f;  //traslate
    fixed_vert = perspective(fixed_vert);

    gl_Position = vec4(fixed_vert, 1.f);
    vert_uv_out = vert_uv;
}
