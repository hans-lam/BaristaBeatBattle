#pragma once

#include "tiny_ecs_registry.hpp"

// Stage System for handling stage switching
class StageSystem
{
public:
	enum Stage {
		main_menu, 
		overworld,
		cutscene, 
		turn_based, 
		minigame
	};

	StageSystem();

	void init();

	Stage get_current_stage() const {
		return this->current_stage;
	}

	bool get_music_changed() const {
		return this->music_changed;
	}

	void set_music_changed();

	void set_stage(Stage target, int level = 1);


	void set_render_shown(Entity entity, bool show_render, bool tutorial);

	void render_clean_up(Entity entity);

	int get_current_level() {
		return this->current_level;
	}

	void set_current_level(int current_level) {
		this->current_level = current_level;
	}

	bool is_london_recruited = false;

private: 
	void set_main_menu();

	void set_overworld(); 

	void set_cutscene(int level_num);

	void set_turn_based(int level_num);

	void set_minigame();

	Stage current_stage;

	int current_level; // THIS HAS NOTHING TO DO WITH CHARACTER LEVEL, THIS IS JUST THE OVERWORLD LEVEL

	bool music_changed;
};