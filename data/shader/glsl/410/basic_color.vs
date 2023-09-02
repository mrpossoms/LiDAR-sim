in vec3 a_position;
in vec2 a_uv;
in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

// uniform mat4 u_light_view;
// uniform mat4 u_light_proj;

out vec2 v_uv;
out vec3 v_normal;
// out vec4 v_light_proj_pos;

void main (void)
{
	vec4 v_world_pos = u_model * vec4(a_position, 1.0);
	vec4 screen_pos = u_proj * u_view * v_world_pos;
	gl_Position = screen_pos / screen_pos.w;

	v_normal = a_normal;
	v_uv = a_uv;

    // mat3 model_rot = mat3(normalize(u_model[0].xyz), normalize(u_model[1].xyz), normalize(u_model[2].xyz));
    // v_normal = normalize(mat3(u_light_view) * model_rot * a_normal);
	// v_light_proj_pos = u_light_proj * u_light_view * v_world_pos;
	// v_light_proj_pos /= v_light_proj_pos.w;
	// v_light_proj_pos = (v_light_proj_pos + 1.0) / 2.0;
}
