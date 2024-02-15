#pragma once

#include "../character_system.hpp"


class CharacterFactory {

public:
	Character* construct_chai();

	Character* construct_earl();

	Character* construct_americano();

	Character* construct_enemy(int level);
};

extern CharacterFactory character_factory;