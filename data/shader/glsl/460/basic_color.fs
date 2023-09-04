in vec3 v_color;

uniform float u_brightness;

out vec4 color;

void main (void)
{
	color.rgb = v_color * u_brightness;
	color.a = 1.0;
	// color = vec4(v_uv, 0.0, 1.0);
}
