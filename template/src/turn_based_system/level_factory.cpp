#pragma once

#include "level_factory.hpp"

Level* LevelFactory::construct_level_one(RenderSystem* renderer, vec2 base_ally_position, vec2 base_enemy_position) {
	Level* level_one = new Level();

	Entity chai_entity = create_chai(renderer, base_ally_position);

	level_one->allies.push_back(chai_entity);

	Entity level_one_ground_a = create_turn_based_enemy(renderer, base_enemy_position, 1);

	level_one->enemies.push_back(level_one_ground_a);

	return level_one;
}

Level* LevelFactory::construct_level_two() {

	Level* level_two = new Level();

	/*
	level_two->allies.push_back(chai_entity);


	level_two->enemies.push_back(americano_entity);
	level_two->enemies.push_back(earl_entity);
	*/

	return level_two;

}

Level* LevelFactory::construct_level_three() {

	Level* level_three = new Level();
	/*
	level_three->allies.push_back(chai_entity);
	level_three->allies.push_back(americano_entity);
	level_three->allies.push_back(earl_entity);


	Entity level_two_ground_a = character_factory.construct_enemy(2);
	Entity level_two_ground_b = character_factory.construct_enemy(2);

	level_three->enemies.push_back(level_two_ground_a);
	level_three->enemies.push_back(level_two_ground_b);
	*/

	return level_three;

}

Level* LevelFactory::construct_level_four() {

	Level* level_four = new Level();
	/*
	level_four->allies.push_back(chai_entity);
	level_four->allies.push_back(americano_entity);
	level_four->allies.push_back(earl_entity);


	Entity level_two_tea_a = character_factory.construct_enemy(2);
	Entity level_two_tea_b = character_factory.construct_enemy(2);
	Entity level_two_tea_c = character_factory.construct_enemy(2);

	level_four->enemies.push_back(level_two_tea_a);
	level_four->enemies.push_back(level_two_tea_b);
	level_four->enemies.push_back(level_two_tea_c);
	*/

	return level_four;

}

Level* LevelFactory::construct_level_five() {

	Level* level_five = new Level();
	/*
	level_five->allies.push_back(chai_entity);
	level_five->allies.push_back(americano_entity);
	level_five->allies.push_back(earl_entity);


	Entity level_two_coffee_a = character_factory.construct_enemy(2);
	Entity level_two_coffee_b = character_factory.construct_enemy(2);
	Entity level_two_coffee_c = character_factory.construct_enemy(2);

	level_five->enemies.push_back(level_two_coffee_a);
	level_five->enemies.push_back(level_two_coffee_b);
	level_five->enemies.push_back(level_two_coffee_c);
	*/

	return level_five;

}

Level* LevelFactory::construct_level_six() {

	Level* level_six = new Level();
	/*
	level_six->allies.push_back(chai_entity);
	level_six->allies.push_back(americano_entity);
	level_six->allies.push_back(earl_entity);


	Entity level_three_ground_a = character_factory.construct_enemy(3);
	Entity level_three_ground_b = character_factory.construct_enemy(3);

	level_six->enemies.push_back(level_three_ground_a);
	level_six->enemies.push_back(level_three_ground_b);
	*/

	return level_six;

}

Level* LevelFactory::construct_level_seven() {

	Level* level_seven = new Level();
	/*
	level_seven->allies.push_back(chai_entity);
	level_seven->allies.push_back(americano_entity);
	level_seven->allies.push_back(earl_entity);


	Entity level_two_coffee_a = character_factory.construct_enemy(2);
	Entity level_two_coffee_b = character_factory.construct_enemy(2);
	Entity ground_devil = character_factory.construct_enemy(5);

	level_seven->enemies.push_back(level_two_coffee_a);
	level_seven->enemies.push_back(level_two_coffee_b);
	level_seven->enemies.push_back(ground_devil);
	*/

	return level_seven;

}