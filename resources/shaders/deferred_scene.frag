#version 450 core
out vec3 frag_color; //GL_COLOR_ATTACHMENT0
out vec3 world_norm; //GL_COLOR_ATTACHMENT1
out vec3 world_pos;  //GL_COLOR_ATTACHMENT2

layout (binding = 0) uniform sampler2D storm_tex; 
in vec2 vert_uv_out;
in vec3 world_pos_out;
in vec3 world_norm_out;

void main() 
{
    frag_color = texture(storm_tex, vert_uv_out).rgb;
    world_norm = normalize(world_norm_out);
    world_pos = world_pos_out;
}