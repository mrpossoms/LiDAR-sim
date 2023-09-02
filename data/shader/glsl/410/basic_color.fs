in vec3 v_normal;
in vec2 v_uv;

out vec4 color;

void main (void)
{
	color.rgb = (v_normal + vec3(1.0)) * 0.5;
	color.rg += v_uv * 0.001;
	color.a = 1.0;
	// color = vec4(v_uv, 0.0, 1.0);
}
