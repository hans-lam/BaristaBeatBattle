
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>

// internal
#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "turn_based_system/turn_based_system.hpp"

using Clock = std::chrono::high_resolution_clock;

// Entry point
int main()
{

	TurnBasedSystem turn_based;

	// Global systems
	WorldSystem world;
	RenderSystem renderer;
	PhysicsSystem physics;

	// Initializing window
	GLFWwindow* window = world.create_window();
	if (!window) {
		// Time to read the error message
		printf("Press any key to exit");
		getchar();
		return EXIT_FAILURE;
	}

	// initialize the main systems
	renderer.init(window);
	turn_based.init();
	world.init(&renderer, &turn_based);
	

	// variable timestep loop
	auto t = Clock::now();
	while (!world.is_over()) {
		// Processes system messages, if this wasn't present the window would become unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto now = Clock::now();
		float elapsed_ms =
			(float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
		t = now;

		world.step(elapsed_ms);
		physics.step(elapsed_ms);
		turn_based.step(elapsed_ms);
		world.handle_collisions();

		if (world.get_stage() == 2) {
			renderer.drawMini();
		}
		else if (world.get_stage() == 1) {
			renderer.drawTurn();
		}
		else {
			renderer.draw();
		}
	}

	return EXIT_SUCCESS;
}
