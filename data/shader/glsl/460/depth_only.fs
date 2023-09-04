// in float v_depth;
in vec4 v_world_pos; 

out vec4 color;

void main (void)
{
    // color.r = v_depth;
    // color = vec4((v_world_pos.xyz + vec3(100.0)) / 200.0, 1.0);

    // color.rgb = v_world_pos.xyz;
    color.rgb = vec3(length(v_world_pos.xyz) / 1000.0, 0.0, 0.0);
    color.a = 1.0;
}
