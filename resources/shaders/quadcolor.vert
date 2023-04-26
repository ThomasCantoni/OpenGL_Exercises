#version 450 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_col;

out vec3 out_vert_col;

void main()
{
    gl_Position = vec4(vert_pos, 1.f);
    out_vert_col = vert_col;
}