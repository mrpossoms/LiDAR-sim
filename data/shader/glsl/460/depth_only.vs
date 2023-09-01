#ifdef GL_ES
precision mediump float;
#endif

in vec3 a_position;
in vec3 a_normal;
in vec3 a_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec4 v_frame_space_pos;

void main (void)
{
    vec4 proj_pos = u_proj * u_view * u_model * vec4(a_position, 1.0);
    gl_Position = proj_pos;

    //proj_pos /= proj_pos.w;
    v_frame_space_pos = (proj_pos);// + 1.0) / 2.0;
}
