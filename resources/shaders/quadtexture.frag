#version 450 core
out vec4 frag_color;

uniform vec4 base_color;

//uniform sampler2D smile_tex; //default binding = 0
layout (binding = 0) uniform sampler2D smile_tex; //1
layout (binding = 1) uniform sampler2D wood_tex; //1

in vec2 vert_uv_out;

void main() 
{
    //frag_color = vec4(1.f, 0.f, 0.f, 1.f);
    //frag_color = base_color;
    vec4 smile_texel = texture(smile_tex, vert_uv_out);
    vec4 wood_texel = texture(wood_tex, vert_uv_out);

    //Like mask effect
    //frag_color = smile_texel * wood_texel;
    frag_color = mix(smile_texel, wood_texel, 0.5f);
}