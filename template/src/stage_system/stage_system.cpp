#include "stage_system.hpp"
#include "tiny_ecs_registry.hpp"
#include <iostream>

StageSystem::StageSystem()
{
	// stub;
}

void StageSystem::init()
{
	current_stage = main_menu;
	music_changed = false;
}

// int level = 0 will make it an optional parameter and default to 1
void StageSystem::set_stage(Stage target, int level)
{
	current_stage = target;

	std::cout << "THIS IS current_level" << level << std::endl;

	switch (current_stage) {
	case main_menu:
		set_main_menu();
		break;
	case overworld:
		set_overworld();
		break;
	case cutscene:
		set_cutscene(level);
		break;
	case turn_based:
		set_turn_based(level);
		break;
	case minigame:
		set_minigame();
		break;
	}

	music_changed = true;
}

void StageSystem::set_music_changed() {
	music_changed = !music_changed;
}

void StageSystem::set_main_menu()
{
	// Do anything here we would need to do if going back to main menu
	// Not sure when/how we go back to main menu at the moment
}

void StageSystem::set_overworld()
{
	// Set all main menu entities to not be shown
	// Keeping entities live right now in case we need them
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

	// Set all entites from turn_based to not be shown
	for (Entity entity : registry.turnBased.entities) {
		if (registry.renderRequests.has(entity)) {
			RenderRequest& render = registry.renderRequests.get(entity);
			render.shown = false;
		}
	}
}

void StageSystem::set_cutscene(int level_num)
{
	current_level = level_num;
	// Set all overworld entities to not be shown
	// Keep entities live since we still might need them
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

void StageSystem::set_turn_based(int level_num)
{
	// Because turn_based always comes after cutscene, use the current_level global variable 
	// to decide which turn_based to initialise/set.
	
	// Remove all components from cutscenes
	for (Entity entity : registry.cutscenes.entities) {
		registry.remove_all_components_of(entity);
	}

	// Set all minigame based entities to not be shown
	for (Entity entity : registry.miniStage.entities) {

	}

	// Set turn based entities to be rendered
	for (Entity entity : registry.turnBased.entities) {
		if (registry.renderRequests.has(entity)) {
			RenderRequest& render = registry.renderRequests.get(entity);
			if (!registry.tutorials.has(entity))
				render.shown = true;
		}
	}
}

void StageSystem::set_minigame()
{
	// Set turn based entities to be rendered
	for (Entity entity : registry.turnBased.entities) {
		if (registry.renderRequests.has(entity)) {
			RenderRequest& render = registry.renderRequests.get(entity);
			if (!registry.tutorials.has(entity))
				render.shown = false;
		}
	}

	// Set minigame based entities to be rendered 
	for (Entity entity : registry.miniStage.entities) {

	}
}