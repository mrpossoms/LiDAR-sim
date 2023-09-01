#include <g.h>

namespace lidar
{

struct sim : public g::core
{

	g::asset::store assets;

	sim();

	virtual bool initialize();

	virtual void update(float dt);

	g::gfx::mesh<vertex::pos_uv_norm> plane;
	g::gfx::mesh<vertex::pos_uv_norm> cube;
	g::gfx::framebuffer lidar_frame;
	g::fps_camera user_camera;
};

}

