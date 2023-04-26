#version 450 core

layout (location = 2) in mat4 inst_mvp;
/* As if:
    layout (location = 2) in vec4 inst_mvp;
    layout (location = 3) in vec4 inst_mvp;
    layout (location = 4) in vec4 inst_mvp;
    layout (location = 5) in vec4 inst_mvp;
 */

in vec3 vert_pos;
in vec2 vert_uv;
out vec2 vert_uv_out;
//uniform mat4 mvp;
//uniform mat4 mvp[8];
out flat int inst_index;

void main()
{
    //gl_Position = mvp * vec4(vert_pos + gl_InstanceID / 4.f, 1.f);
    //gl_Position = mvp[gl_InstanceID] * vec4(vert_pos, 1.f);
    
    gl_Position = inst_mvp * vec4(vert_pos, 1.f);
    vert_uv_out = vert_uv;

    inst_index = gl_InstanceID;
}