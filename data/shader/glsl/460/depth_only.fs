in vec4 v_frame_space_pos;

out vec4 color;

void main (void)
{
    color = v_frame_space_pos;
    // color = vec4(vec3(gl_FragCoord.z), 1.0);
}
