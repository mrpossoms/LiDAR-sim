#include "g.h"
#include "state.hpp"
#include "lidar/sim.hpp"

#ifdef __EMSCRIPTEN__
EM_JS(int, canvas_get_width, (), {
  return document.getElementById('canvas').width;
});

EM_JS(int, canvas_get_height, (), {
  return document.getElementById('canvas').height;
});
#endif

int main (int argc, const char* argv[])
{
	lidar::sim sim;

	g::core::opts opts = {
		.name = "LiDAR sim demo",
		.gfx = {
			.display = true,
			.width = 1024,
			.height = 768 
		}
	};

#ifdef __EMSCRIPTEN__
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	opts.gfx.width = canvas_get_width();
	opts.gfx.height = canvas_get_height();
#else
	opts.gfx.fullscreen = false;
#endif

	sim.start(opts);

	return 0;
}
