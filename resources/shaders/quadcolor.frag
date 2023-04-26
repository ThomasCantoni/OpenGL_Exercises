#version 450 core
out vec4 frag_color;

uniform vec4 base_color;

in vec3 out_vert_col;

void main() 
{
    //frag_color = vec4(1.f, 0.f, 0.f, 1.f);
    //frag_color = base_color;
    frag_color = vec4(out_vert_col, 1.f);
}