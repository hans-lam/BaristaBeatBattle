#include "stage_system.hpp"

#include "tiny_ecs_registry.hpp"
#include <iostream>



StageSystem::StageSystem()
{
	current_level = 1;
	// stub;
}

void StageSystem::init()
{
	current_level = 1;
	current_stage = main_menu;
	music_changed = false;
}

// int level = 0 will make it an optional parameter and default to 1
void StageSystem::set_stage(Stage target, int level)
{

	if ((target != turn_based)) {
		this->current_level = level;

	}
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
	case cutscene_ending:
		set_cutscene_ending(level);
		break;
	case cutscene_before:
		set_cutscene_before(level);
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

void StageSystem::set_render_shown(Entity entity, bool show_render, bool tutorial) {

	if (registry.renderRequests.has(entity)) {
		RenderRequest& render = registry.renderRequests.get(entity);
		if (tutorial) {
			if (!registry.tutorials.has(entity))
				render.shown = show_render;
		}
		else {
			render.shown = show_render;
		}
	}
	else if (registry.textRenderRequests.has(entity)) {
		TextRenderRequest& textRender = registry.textRenderRequests.get(entity);
		textRender.shown = show_render;
	}
}

void StageSystem::render_clean_up(Entity entity) {
	if (registry.renderRequests.has(entity)) {
		RenderRequest& rr = registry.renderRequests.get(entity);
		if (rr.isStatic) {
			set_render_shown(entity, false, false);
		}
		else {
			registry.remove_all_components_of(entity);
		}
	}
	else if (registry.textRenderRequests.has(entity)) {
		TextRenderRequest& trr = registry.textRenderRequests.get(entity); 
		if (trr.isStatic) {
			set_render_shown(entity, false, false);
		}
		else {
			registry.remove_all_components_of(entity);
		}
	}
}

void StageSystem::set_main_menu()
{
	// Set all main menu entities to be shown 
	for (Entity entity : registry.mainMenu.entities) {
		set_render_shown(entity, true, false);
	}

	// Set all overworld entities to not be shown
	for (Entity entity : registry.overWorld.entities) {
		render_clean_up(entity);
	}

	// Set all entites from turn_based to not be shown
	for (Entity entity : registry.turnBased.entities) {
		render_clean_up(entity);
	}

	// Set all entities from cutscene to not be shown
	for (Entity entity : registry.cutscenes.entities) {
		render_clean_up(entity);
	}

	// Set turn based entities to not be rendered
	for (Entity entity : registry.turnBased.entities) {
		render_clean_up(entity);
	}

	// Set all minigame based entities to not be shown
	for (Entity entity : registry.miniStage.entities) {
		render_clean_up(entity);
	}
	for (Entity entity : registry.miniGame.entities) {
		render_clean_up(entity);
	}
}

void StageSystem::set_overworld()
{
	// Set all main menu entities to not be shown
	// Keeping entities live right now in case we need them
	for (Entity entity : registry.mainMenu.entities) {
		set_render_shown(entity, false, false);
	}

	//setting all cutscene entities to false 
	for (Entity entity : registry.cutscenes.entities) {
		set_render_shown(entity, false, false);
	}

	// Set all overworld entities to be shown
	for (Entity entity : registry.overWorld.entities) {
		set_render_shown(entity, true, true);
	} 

	// Set all entites from turn_based to not be shown
	for (Entity entity : registry.turnBased.entities) {
		render_clean_up(entity);
	}
}

void StageSystem::set_cutscene(int level_num)
{
	current_level = level_num;
	// Set all overworld entities to not be shown
	// Keep entities live since we still might need them
	for (Entity entity : registry.overWorld.entities) {
		render_clean_up(entity);
	}

	// Set all cutscene entities to be shown; 
	// this can be changed depending on how we want to decide which cutscenes are rendered
	for (Entity entity : registry.cutscenes.entities) {
		set_render_shown(entity, true, false);
	}
}

void StageSystem::set_cutscene_before(int level_num)
{
	current_level = level_num;
	// Set all overworld entities to not be shown
	// Keep entities live since we still might need them
	for (Entity entity : registry.mainMenu.entities) {
		set_render_shown(entity, false, false);
	}

	// Set all cutscene entities to be shown; 
	// this can be changed depending on how we want to decide which cutscenes are rendered
	for (Entity entity : registry.cutscenes.entities) {
		set_render_shown(entity, true, false);
	}
}

void StageSystem::set_cutscene_ending(int level_num)
{
	current_level = level_num;
	// Set all overworld entities to not be shown
	// Keep entities live since we still might need them
	for (Entity entity : registry.mainMenu.entities) {
		set_render_shown(entity, false, false);
	}

	// Set all cutscene entities to be shown; 
	// this can be changed depending on how we want to decide which cutscenes are rendered
	for (Entity entity : registry.cutscenes.entities) {
		set_render_shown(entity, true, false);
	}
}

void StageSystem::set_turn_based(int level_num)
{
	// Because turn_based always comes after cutscene, use the current_level global variable 
	// to decide which turn_based to initialise/set.
	
	// Remove all components from cutscenes
	for (Entity entity : registry.cutscenes.entities) {
		render_clean_up(entity);
	}

	// Set all minigame based entities to not be shown
	for (Entity entity : registry.miniStage.entities) {
		render_clean_up(entity);
	}
	for (Entity entity : registry.miniGame.entities) {
		render_clean_up(entity);
	}

	// Set turn based entities to be rendered
	for (Entity entity : registry.turnBased.entities) {
		set_render_shown(entity, true, true);
	}
}

void StageSystem::set_minigame()
{
	// Set turn based entities to be rendered
	for (Entity entity : registry.turnBased.entities) {
		set_render_shown(entity, false, false);
		if (registry.missTimers.has(entity)) {
			set_render_shown(registry.missTimers.get(entity).associated_text, false, false);
		}
		else if (registry.levelUpTimers.has(entity)) {
			set_render_shown(registry.levelUpTimers.get(entity).associated_text, false, false);
		}
	}

	// Set minigame based entities to be rendered 
	for (Entity entity : registry.miniStage.entities) {
		set_render_shown(entity, true, false);
	}
}