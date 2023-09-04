in vec3 a_position;
in vec3 a_normal;
in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

// out float v_depth;
out vec4 v_world_pos;
out vec2 v_uv;

void main (void)
{
    v_world_pos = u_view * u_model * vec4(a_position, 1.0);
    v_uv = a_uv;
    // v_depth = v_world_pos.xyz; // / 1000.0;
    gl_Position = u_proj * v_world_pos;
}
