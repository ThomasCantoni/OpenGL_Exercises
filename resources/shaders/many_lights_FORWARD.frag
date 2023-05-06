#version 450 core
out vec4 frag_color;

layout (binding = 0) uniform sampler2D storm_tex; 
in vec2 vert_uv_out;
in vec3 vert_norm_out;
in vec3 world_pos_out;
in vec3 world_norm_out;


uniform vec3 camera_pos;
struct light_t {
    vec3 pos;
    vec3 color;
    vec3 specular_color;
    float specular_factor;

};
uniform  int light_num; 
uniform light_t lights[200];
 


vec3 phong2(light_t current_light) 
{
    vec3 mesh_color = texture(storm_tex, vert_uv_out).rgb;
    

    //Ambient
    float ambient_factor = 0.2f;
    vec3 ambient = mesh_color * ambient_factor;

    //Diffuse
    vec3 light_dir = normalize(current_light.pos - world_pos_out);
    vec3 world_norm = normalize(world_norm_out);
    float lambert = max(dot(world_norm, light_dir), 0.f);
    vec3 diffuse = mesh_color * lambert * current_light.color;

    //Specular
    vec3 eye_dir = normalize(camera_pos - world_pos_out);
    vec3 refl_dir = reflect(-light_dir, world_norm);
    float spec_str = max(dot(eye_dir, refl_dir), 0.f);
    float spec_fact = pow(spec_str, 40.f);
    vec3 specular = current_light.specular_color *spec_fact * current_light.specular_factor;

    vec3 phong = vec3(0.f);
    phong += ambient;
    phong += diffuse;
    phong += specular;

    /*
        attention = 1.f / contribute;
            Kc = 1.f;
            Kl => dependends on distance
            Kq => dependends on distance

        contribute = Kc + Kl * distance + Kq * distance * distance
     */
    
    float k_c = 1.f;
    float k_l = 0.35f;
    float k_q = 0.44f;
    float dist = length(current_light.pos - world_pos_out);

    float attenuation = 1.f / ( k_c + k_l * dist + k_q * dist * dist );
    phong *= attenuation;

    return phong;
}
void main() 
{
    vec3 phong_color = vec3(0);

   for(int i=0;i<light_num;i++)
   {
    phong_color += phong2(lights[i]);

   }
    
   
    frag_color = vec4(phong_color, 1.f);
}