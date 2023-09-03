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

	void draw_scene(const g::game::camera& cam);

	g::gfx::mesh<vertex::pos_uv_norm> ground;
	g::gfx::mesh<vertex::pos_uv_norm> cube;
	g::gfx::texture lidar_frame;
	g::gfx::framebuffer lidar_fb;
	g::game::camera_perspective lidar_camera;
	g::gfx::vertex::pos* lidar_rays_basis;
	g::gfx::vertex::pos* lidar_rays;
	g::gfx::mesh<vertex::pos> lidar_point_cloud;
	vec<2, unsigned> lidar_res;
	g::game::fps_camera user_camera;
};

}

