in vec3 a_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 v_color;

void main (void)
{
    vec4 v_world_pos = u_model * vec4(a_position, 1.0);
    vec4 screen_pos = u_proj * u_view * v_world_pos;

    v_color = vec3(1.0, length(a_position)/500.0, 0.0);
    gl_Position = screen_pos;
}
