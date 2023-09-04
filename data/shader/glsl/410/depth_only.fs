// in float v_depth;
in vec4 v_world_pos; 
in vec2 v_uv;

uniform sampler2D u_lidar_noise;
uniform float u_time;

out vec4 color;

void main (void)
{
    // color.r = v_depth;
    // color = vec4((v_world_pos.xyz + vec3(100.0)) / 200.0, 1.0);

    // color.rgb = v_world_pos.xyz;
    float noise = texture(u_lidar_noise, v_uv + vec2(u_time)).r;

    color.rgb = vec3(length(v_world_pos.xyz) - 0.05 + noise, 0.0, 0.0);
    color.a = 1.0;
}
