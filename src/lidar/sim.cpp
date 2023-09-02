
#include "lidar/sim.hpp"

lidar::sim::sim()
{

}

bool lidar::sim::initialize()
{
	cube = g::gfx::mesh_factory::cube(); 
	// ground = g::gfx::mesh_factory::plane(); //{0, 1, 0}, {100, 100});

    auto vertex_generator = [](const texture& tex, int x, int y) -> vertex::pos_uv_norm {
        return {
            // position
            {
                x - tex.size[0] * 0.5f,
                static_cast<float>(tex.sample(x, y)[0] * 0.25f),
                y - tex.size[1] * 0.5f,
            },
            // uv
            {
                x / (float)tex.size[0],
                y / (float)tex.size[1],
            },
            // normal
            { 0, 1, 0 }
        };
    };
    ground = g::gfx::mesh_factory::from_heightmap(assets.tex("heightmap.png")); //<vertex::pos_uv_norm>(assets.tex("heightmap.png"), vertex_generator);


	// lidar_frame = framebuffer_factory{ 1024, 128 }.depth().create();

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
    // glfwSetInputMode(g::gfx::GLFW_WIN, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glCullFace(GL_FRONT);
    glClearColor(0.5, 0.5, 1.0, 1.0);

	return true;
}

void lidar::sim::update(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    user_camera.aspect_ratio(g::gfx::aspect());
    user_camera.pre_update(dt, 0);
    user_camera.update(dt, 0);

	ground.using_shader(assets.shader("basic_color.vs+basic_color.fs"))
	.set_camera(user_camera)
	["u_model"].mat4(mat4::translation({0, 0, 0}) * mat4::scale({1, 0.1, 1}))
	.draw<GL_POINTS>();

	cube.using_shader(assets.shader("basic_color.vs+basic_color.fs"))
	.set_camera(user_camera)
    ["u_model"].mat4(mat4::translation({0, 4, 0}))
	.draw<GL_TRIANGLES>();		


}