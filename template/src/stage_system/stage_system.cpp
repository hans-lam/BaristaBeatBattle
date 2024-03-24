#include "stage_system.hpp"
#include "tiny_ecs_registry.hpp"

StageSystem::StageSystem()
{
	// stub;
}

void StageSystem::init()
{
	current_stage = main_menu;
}

void StageSystem::set_stage(Stage target)
{
	current_stage = target;

	switch (current_stage) {
	case main_menu:
		set_main_menu();
		break;
	case overworld:
		set_overworld();
		break;
	case cutscene:
		set_cutscene();
		break;
	case turn_based:
		set_turn_based();
		break;
	case minigame:
		set_minigame();
		break;
	}
}

void StageSystem::set_main_menu()
{

}

void StageSystem::set_overworld()
{
	// Set all main menu entities to not be shown
	for (Entity entity : registry.mainMenu.entities) {
		RenderRequest& render = registry.renderRequests.get(entity);
		render.shown = false;
	}

	// Set all overworld entities to be shown
	for (Entity entity : registry.overWorld.entities) {
		RenderRequest& render = registry.renderRequests.get(entity);
		if (!registry.tutorials.has(entity))
			render.shown = true;
	}
}

void StageSystem::set_cutscene()
{
	// Set all overworld entities to not be shown
	for (Entity entity : registry.overWorld.entities) {
		RenderRequest& render = registry.renderRequests.get(entity);
		render.shown = false;
	}

	// Set all cutscene entities to be shown; 
	// this can be changed depending on how we want to decide which cutscenes are rendered
	for (Entity entity : registry.cutscenes.entities) {
		RenderRequest& render = registry.renderRequests.get(entity);
		render.shown = true;
	}
}

void StageSystem::set_turn_based()
{

}

void StageSystem::set_minigame()
{

}