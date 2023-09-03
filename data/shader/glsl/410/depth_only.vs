in vec3 a_position;
in vec3 a_normal;
in vec3 a_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out float v_depth;

void main (void)
{
    vec4 world_space = u_view * u_model * vec4(a_position, 1.0);
    v_depth = length(world_space.xyz);
    gl_Position = u_proj * world_space;
}
