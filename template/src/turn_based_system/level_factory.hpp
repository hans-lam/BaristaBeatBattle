#pragma once

#include "./character_factory/character_factory.hpp"
#include "turn_based_system.hpp"
#include "level_select_system.hpp"
#include "flag_progression.hpp"

class LevelFactory {
public:
	LevelFactory();

	Level* construct_level_one(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position); 

	Level* construct_level_two(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position);

	Level* construct_level_three(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position);

	Level* construct_level_four(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position);

	Level* construct_level_five(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position);

	Level* construct_level_six(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position);

	Level* construct_level_seven(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position);
};