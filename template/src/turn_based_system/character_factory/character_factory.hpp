#pragma once

#include "../character_system.hpp"
#include "../../tiny_ecs_registry.hpp"
#include "../ability_factory/ability_factory.hpp"

class CharacterFactory {

public:
	Entity construct_chai();

	Entity construct_earl();

	Entity construct_americano();

	Entity construct_enemy(int level);
};

extern CharacterFactory character_factory;