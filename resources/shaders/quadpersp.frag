#version 450 core
out vec4 frag_color;

layout (binding = 0) uniform sampler2D smile_tex; 
layout (binding = 1) uniform sampler2D wood_tex; 

in vec2 vert_uv_out;

void main() 
{
    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 wood_texel = texture(wood_tex, vert_uv_out);
    frag_color = mix(smile_texel, wood_texel, 0.5f);
}