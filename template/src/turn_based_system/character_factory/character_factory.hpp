#pragma once

#include "../character_system.hpp"
#include "../../tiny_ecs_registry.hpp"
#include "../ability_factory/ability_factory.hpp"

class CharacterFactory {

public:
	Entity construct_chai(Entity entity);

	Entity construct_earl(Entity entity);

	Entity construct_americano(Entity entity);

	Entity construct_london(Entity entity);

	Entity construct_enemy(Entity entity, int level);

	Entity construct_tutorial_enemy(Entity entity);
};

int does_character_data_already_exist(Entity entity, std::string name);

extern CharacterFactory* character_factory;