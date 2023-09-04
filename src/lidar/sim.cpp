
#include "lidar/sim.hpp"

void unit_sphere_basis(
	const vec<2, unsigned>& lidar_res,
	const g::game::camera_perspective& lidar_camera,
	g::gfx::vertex::pos_uv* basis)
{
	auto fov_h = lidar_camera.field_of_view;
	auto fov_v = lidar_camera.field_of_view;

	for (int r = 0; r < lidar_res[0]; r++)
	{
		float el = (r / (float)lidar_res[0]) * fov_v - (fov_v / 2);
		auto R_e = mat4::rotation({1, 0, 0}, el);

		for (int c = 0; c < lidar_res[1]; c++)
		{
			float az = (c / (float)lidar_res[1]) * fov_h - (fov_h / 2);
			auto R_az = mat4::rotation({0, 1, 0}, az);
			basis[r * lidar_res[1] + c].position = R_az * R_e * vec<3>{0, 0, -1};
			basis[r * lidar_res[1] + c].uv = {az, el};
		}
	}
}

void rectilinear_basis(
	const vec<2, unsigned>& lidar_res,
	const g::game::camera_perspective& lidar_camera,
	g::gfx::vertex::pos_uv* basis)
{
	auto fov_h = lidar_camera.field_of_view;
	auto fov_v = lidar_camera.field_of_view;

	auto P = lidar_camera.projection();
	auto P_inv = P.invert();
	auto projected = P * vec<4>{0, 0, lidar_camera.far, 1.f};
	auto w = projected[3]; 

	for (int r = 0; r < lidar_res[0]; r++)
	{
		auto r_p = r / (float)lidar_res[0];
		r_p = (r_p * 2) - 1;

		for (int c = 0; c < lidar_res[1]; c++)
		{
			auto c_p = c / (float)lidar_res[1];
			c_p = (c_p * 2) - 1;

			auto unprojected = P_inv * vec<4>{c_p, r_p, 1.f, w};

			basis[r * lidar_res[1] + c].position = vec<3>{c_p, -r_p, -1.f}.unit();
			basis[r * lidar_res[1] + c].uv = {c_p, r_p};
		}
	}
}



lidar::sim::sim() : lidar_res({ 128, 128 })
{
	auto fov_h = lidar_camera.field_of_view;
	auto fov_v = lidar_camera.field_of_view;

	lidar_rays_basis = new g::gfx::vertex::pos_uv[lidar_res[0] * lidar_res[1]];
	lidar_rays = new g::gfx::vertex::pos_uv[lidar_res[0] * lidar_res[1]];

	rectilinear_basis(lidar_res, lidar_camera, lidar_rays_basis);
}

bool lidar::sim::initialize()
{
	cube = g::gfx::mesh_factory::cube(); 
    ground = g::gfx::mesh_factory::from_heightmap(assets.tex("heightmap.png"));

    lidar_frame = texture_factory{ lidar_res[0], lidar_res[1] }.type(GL_FLOAT).components(4).pixelated().create();
	lidar_fb = framebuffer_factory{ lidar_frame }.depth().create();
	lidar_point_cloud = g::gfx::mesh_factory::empty_mesh<vertex::pos_uv>();
	lidar_camera.aspect_ratio(lidar_frame.aspect());
	lidar_camera.position = {0, 10, 10};

	std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution d{0.f, 0.1f};
 
	lidar_noise = g::gfx::texture_factory{256, 256}.components(4)
	                                               .type(GL_FLOAT)
	                                               .smooth()
	                                               .fill([&](unsigned x, unsigned y, unsigned z, unsigned char* data){
	                                                    ((float*)data)[0] = d(gen);
	                                                })
	                                               .repeating()
	                                               .create();

	user_camera.gravity = {};
	user_camera.position = {0, 10, 0};
	user_camera.on_input = [](fps_camera& cam, float dt){
        static double xlast, ylast;
        float sensitivity = 0.5f;
        double xpos = 0, ypos = 0;
        auto mode = glfwGetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR);

        if (GLFW_CURSOR_DISABLED == mode)
        {
            glfwGetCursorPos(g::gfx::GLFW_WIN, &xpos, &ypos);
        }

        if (glfwGetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        if (xlast != 0 || ylast != 0)
        {
            auto dx = xpos - xlast;
            auto dy = ypos - ylast;
            cam.pitch += (dy * dt * sensitivity);
            cam.yaw += (-dx * dt * sensitivity);
        }

        xlast = xpos; ylast = ypos;

        auto speed = cam.speed;
        if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_W) == GLFW_PRESS) cam.velocity += cam.body_forward() * speed;
        if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_S) == GLFW_PRESS) cam.velocity += cam.body_forward() * -speed;
        if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_A) == GLFW_PRESS) cam.velocity += cam.body_left() * speed;
        if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_D) == GLFW_PRESS) cam.velocity += cam.body_left() * -speed;
        if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_SPACE) == GLFW_PRESS) cam.velocity += cam.body_up() * speed;
        if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) cam.velocity -= cam.body_up() * speed;
        if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
        cam.velocity += -cam.velocity * dt * 5.0f;

        if (glfwGetMouseButton(g::gfx::GLFW_WIN, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    };

    glClearColor(0.25, 0.25, 0.25, 1.0);
    glPointSize(2);

	return true;
}

void lidar::sim::draw_scene(g::gfx::shader& shader, const g::game::camera& cam, float t)
{
	ground.using_shader(shader)
	.set_camera(cam)
	["u_brightness"].flt(0.25)
	["u_model"].mat4(mat4::translation({0, 0, 0}) * mat4::scale({1, 0.1, 1}))
	["u_lidar_noise"].texture(lidar_noise)
	["u_time"].flt(t)
	.draw<GL_TRIANGLES>();

	cube.using_shader(shader)
	.set_camera(cam)
	["u_brightness"].flt(0.25)
    ["u_model"].mat4(mat4::translation({0, 4, -15}))
    ["u_lidar_noise"].texture(lidar_noise)
	["u_time"].flt(t)
	.draw<GL_TRIANGLES>();
}

static void print_depths(const float* depths, const texture& lidar_frame)
{
	for (unsigned r = 0; r < lidar_frame.size[0]; r++)
	{
		for (unsigned c = 0; c < lidar_frame.size[1]; c++)
		{
			std::cout << depths[r * lidar_frame.size[1] + c] << " ";
		}		
		std::cout << std::endl;
	}
	std::cout << "--------" << std::endl;
}

float t = 0;

void lidar::sim::update(float dt)
{
	static float* lidar_depths = nullptr;

	if (lidar_depths == nullptr)
	{
		lidar_depths = new float[lidar_frame.size[0] * lidar_frame.size[1]];
	}

	{
		g::gfx::framebuffer::scoped_draw fb(lidar_fb);
		glReadPixels(0, 0, lidar_frame.size[0], lidar_frame.size[1], GL_RED, GL_FLOAT, lidar_depths);
		// print_depths(lidar_depths, lidar_frame);

        float min = 1000, max = 0;

        // depth only
		for (unsigned r = 0; r < lidar_frame.size[0]; r++)
		{
			for (unsigned c = 0; c < lidar_frame.size[1]; c++)
			{
				auto d = lidar_depths[((lidar_frame.size[0]-1) - r) * lidar_frame.size[1] + c];// * 1000;
                min = std::min(min, d);
                max = std::max(max, d);
				lidar_rays[r * lidar_frame.size[1] + c].position = lidar_rays_basis[r * lidar_frame.size[1] + c].position * d;
			}
		}
		lidar_point_cloud.set_vertices(lidar_rays, lidar_frame.size[0] * lidar_frame.size[1]);

        std::cout << "min: " << min << " max: " << max << std::endl;
	}


    glClearColor(0.25, 0.25, 0.25, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    user_camera.aspect_ratio(g::gfx::aspect());
    user_camera.pre_update(dt, 0);
    user_camera.update(dt, 0);

    g::game::camera* render_cam = &user_camera;

    if (glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_C)) { render_cam = &lidar_camera; }

    auto& vis_shader = assets.shader("basic_color.vs+basic_color.fs");
    draw_scene(vis_shader, *render_cam, t);

    bool depth_test = !glfwGetKey(g::gfx::GLFW_WIN, GLFW_KEY_V);
    
    if (!depth_test) glDisable(GL_DEPTH_TEST);
    lidar_point_cloud.using_shader(assets.shader("pointcloud.vs+basic_color.fs"))
    .set_camera(*render_cam)
    ["u_brightness"].flt (1.f)
    ["u_model"].mat4(lidar_camera.view().invert())
    .draw<GL_POINTS>();
    if (!depth_test) glEnable(GL_DEPTH_TEST);

    {
    	g::gfx::framebuffer::scoped_draw fb(lidar_fb);
	    glClearColor(0, 0.25, 0.25, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	draw_scene(assets.shader("depth_only.vs+depth_only.fs"), lidar_camera, t);
    }

    t += dt;
}