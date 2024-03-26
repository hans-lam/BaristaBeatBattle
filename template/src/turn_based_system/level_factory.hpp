#pragma once

#include "./character_factory/character_factory.hpp"
#include "turn_based_system.hpp"
#include "level_select_system.hpp"

class LevelFactory {
public:
	Level* construct_level_one(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position); 

	Level* construct_level_two();

	Level* construct_level_three();

	Level* construct_level_four();

	Level* construct_level_five();

	Level* construct_level_six();

	Level* construct_level_seven();
};