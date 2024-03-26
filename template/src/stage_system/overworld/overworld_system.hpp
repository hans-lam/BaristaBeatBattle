#pragma once
#include "stage_system/stage_system.hpp"

// System for overworld
class OverworldSystem
{
public:
	OverworldSystem(); 

	void init(StageSystem* stage_system_arg);

	void handle_overworld_keys(int key, int action, float player_speed); 

private:
	bool overworld_tutorial;
	StageSystem* stage_system;

	// helpers for button presses
	void handle_player_movement(int key, int action, float player_speed); 
	void player_attack();
	void handle_tutorial();

};