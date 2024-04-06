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
	void updatePlayerVelocityTowardsTarget(float elapsed_ms);
	std::pair<float, float> getBezierPath(float start_x, float start_y, float end_x, float end_y, float time);

	LevelNode nearest_left_node;
	LevelNode nearest_right_node;
	
	LevelNode nearest_node; // used to generalize omg im so sorry this is realyl bad code
	LevelNode prev_node; //used in the calcualtion of bezier movement
	float dist_remaining; // used to keep track of how much progress has been made in the bezier movement. 
	float stored_static_dist_remaining; // used to normalize the dist_remaining by dividing a decreasing dist_remaining with stored_static_dist_remaining
	

private:
	bool overworld_tutorial;
	bool is_target_reached;
	StageSystem* stage_system;
	RenderSystem* renderer;
	int current_level; // gets updated when handle_movement receives a key press
	int prev_level; // stores prev level
	bool step();
	//void create_overworld_levels();
	

	// helpers for button presses
	void handle_player_movement(int key, int action, float player_speed); 
	//void player_attack();
	void handle_level_selection();
	void handle_tutorial();

};