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

	void set_stage(Stage target);

	void set_render_shown(Entity entity, bool show_render, bool tutorial);
private: 
	void set_main_menu();

	void set_overworld(); 

	void set_cutscene();

	void set_turn_based();

	void set_minigame();

	Stage current_stage;

	bool music_changed;
};