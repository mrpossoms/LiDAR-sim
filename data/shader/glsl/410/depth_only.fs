in float v_depth;

out vec4 color;

void main (void)
{
    color.r = v_depth;
    // color = vec4(vec3(gl_FragCoord.z), 1.0);
}
