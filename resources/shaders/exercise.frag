#version 450 core
out vec4 frag_color;

layout (binding = 0) uniform sampler2D storm_tex; 
in vec2 vert_uv_out;
in vec3 vert_norm_out;
in vec3 world_pos_out;
in vec3 world_norm_out;

uniform vec3 point_light_pos;
uniform vec3 camera_pos;

void main() 
{
    vec4 storm_texel = texture(storm_tex, vert_uv_out);
    //frag_color = storm_texel;
    //frag_color = vec4(abs(vert_norm_out), 1.f);
    //frag_color = vec4(abs(world_norm_out), 1.f);

    vec3 mesh_color = storm_texel.xyz;

    //Ambient
    float ambient_factor = 0.2f;
    vec3 ambient = mesh_color * ambient_factor;

    //Diffuse
    vec3 light_dir = normalize(point_light_pos - world_pos_out);
    vec3 world_norm = normalize(world_norm_out);
    float lambert = max(dot(world_norm, light_dir), 0.f);
    vec3 diffuse = mesh_color * lambert;

    //Specular
    vec3 eye_dir = normalize(camera_pos - world_pos_out);
    vec3 refl_dir = reflect(-light_dir, world_norm);
    float spec_str = max(dot(eye_dir, refl_dir), 0.f);
    float spec_fact = pow(spec_str, 40.f);
    vec3 specular = vec3(1.f, 1.f, 1.f) * spec_fact;

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
    float dist = length(point_light_pos - world_pos_out);

    float attenuation = 1.f / ( k_c + k_l * dist + k_q * dist * dist );
    phong *= attenuation;

    frag_color = vec4(phong, 1.f);
}