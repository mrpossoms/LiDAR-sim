
#include "lidar/sim.hpp"

lidar::sim::sim()
{

}

bool lidar::sim::initialize()
{
	cube = g::gfx::mesh_factory::cube(); 
	plane = g::gfx::mesh_factory::plane({0, 1, 0}, {100, 100});

	lidar_frame = g::gfx::framebuffer_factory{ 1024, 16 }.depth().create();

	return true;
}

void lidar::sim::update(float dt)
{
	glClearColor(0.5, 0.5, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}