
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>

// internal
#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "turn_based_system/turn_based_system.hpp"
#include "stage_system/stage_system.hpp"
#include "stage_system/main_menu/main_menu_system.hpp"
#include "stage_system/overworld/overworld_system.hpp"
#include "stage_system/cutscene/cutscene_system.hpp"
#include "stage_system/combat/combat_system.hpp"
#include "stage_system/combat/minigame_system.hpp"

using Clock = std::chrono::high_resolution_clock;

// Entry point
int main()
{
	AISystem ai_system;
	TurnBasedSystem turn_based;
	StageSystem stage_system;
	MainMenuSystem main_menu_system;
	OverworldSystem overworld_system;
	CutSceneSystem cutscene_system;
	CombatSystem combat_system;
	MinigameSystem minigame_system;

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
	turn_based.init(&ai_system);
	stage_system.init();
	world.init(&renderer, &turn_based, &stage_system, &main_menu_system, 
		&overworld_system, &cutscene_system, &combat_system, &minigame_system);

	// setup fonts 
	std::string font_filename = "..//..//..//data//fonts//Kenney_Pixel_Square.ttf";
	unsigned int font_default_size = 48;
	renderer.fontInit(font_filename, font_default_size);

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
		world.handle_level_collisions();
		renderer.draw(stage_system.get_current_stage());
	}

	return EXIT_SUCCESS;
}
