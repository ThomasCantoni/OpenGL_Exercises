#version 450 core
layout (location = 0) in vec3 vert_pos;
//layout (location = 1)  in vec2 vert_uv;
//layout (location = 2)  in vec3 vert_norm;
layout (location = 3) in mat4 InstanceMatrix;




//uniform mat4 mvp;

out flat int instance_index;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{
   //gl_Position = InstMvp;
    gl_Position = InstanceMatrix*vec4(vert_pos, 1.f);  
   // instance_index = gl_InstanceID;
    //vert_norm_out = vert_norm;
   // world_pos_out = ( InstMvp * vec4(vert_pos, 1.f) ).xyz;
   //gl_Position=( InstMvp * vec4(vert_pos, 1.f) );
   //gl_Position= mat3(transpose(inverse(InstMvp))) * vert_norm;
    
}
