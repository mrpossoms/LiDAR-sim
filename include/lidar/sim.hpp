#include <g.h>

using mat4 = xmath::mat<4, 4>;

namespace lidar
{

struct sim : public g::core
{

	g::asset::store assets;

	sim();

	virtual bool initialize();

	virtual void update(float dt);

	g::gfx::mesh<vertex::pos_uv_norm> ground;
	g::gfx::mesh<vertex::pos_uv_norm> cube;
	// g::gfx::framebuffer lidar_frame;
	g::game::fps_camera user_camera;
};

}

