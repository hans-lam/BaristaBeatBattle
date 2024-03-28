#pragma once
#include "stage_system/stage_system.hpp"
#include <render_system.hpp>

// Justin: NOTE THAT SOME OF THE FUNCTOINALITY IS ALSO IN world_system.cpp

// System for overworld
class OverworldSystem
{
public:
	OverworldSystem(); 

	void init(StageSystem* stage_system_arg);

	void handle_overworld_keys(int key, int action, float player_speed); 

private:
	bool overworld_tutorial;
	bool is_target_reached;
	StageSystem* stage_system;
	RenderSystem* renderer;
	int current_level;
	bool step();
	//void create_overworld_levels();
	void updatePlayerVelocityTowardsTarget(Motion& player_motion, LevelNode* target_node, float player_speed);


	// helpers for button presses
	void handle_player_movement(int key, int action, float player_speed); 
	//void player_attack();
	void handle_level_selection();
	void handle_tutorial();

};