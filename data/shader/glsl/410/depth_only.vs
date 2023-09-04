in vec3 a_position;
in vec2 a_uv;
in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out float v_depth;

void main (void)
{
    vec4 world_space = u_view * u_model * vec4(a_position, 1.0);
    // v_depth = min(1.0, length(world_space.xyz) / 1000.0);
    gl_Position = u_proj * world_space;
}
