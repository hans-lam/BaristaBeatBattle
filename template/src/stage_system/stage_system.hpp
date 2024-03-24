#pragma once

// A simple physics system that moves rigid bodies and checks for collision
class StageSystem
{
public:
	enum stage {
		main_menu, 
		overworld,
		cutscene, 
		turn_based, 
		minigame
	};

	StageSystem();

	stage get_current_stage() const {
		return this->current_stage;
	}

	void set_stage(stage target);
private: 
	void set_main_menu();

	void set_overworld(); 

	void set_cutscene();

	void set_turn_based();

	void set_minigame();

	stage current_stage; 
};