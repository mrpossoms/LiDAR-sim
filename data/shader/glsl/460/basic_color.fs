in vec3 v_normal;
in vec2 v_uv;

out vec4 color;

void main (void)
{
	// color.rgb = vec3(1.0, 0.0, 0.0);//(v_normal + vec3(1.0)) * 0.5;
	// color.a = 1;
	color = vec4(v_uv, 0.0, 1.0);
}
