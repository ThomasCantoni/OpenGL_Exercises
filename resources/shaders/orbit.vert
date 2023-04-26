#version 450 core
layout (location = 0)  in vec3 vert_pos;
//layout (location = 1)  in vec2 vert_uv;
//layout (location = 2)  in vec3 vert_norm;
layout (location = 3) in mat4 InstMvp;





uniform model;
out vec3 vert_norm_out;
out vec3 world_pos_out;
out vec3 world_norm_out;

void main()
{
    
    gl_Position = InstMvp * vec4(vert_pos, 1.f);
   
    //vert_norm_out = vert_norm;
    world_pos_out = ( InstMvp * vec4(vert_pos, 1.f) ).xyz;
   // world_norm_out = mat3(transpose(inverse(InstMvp))) * vert_norm;
    
}
