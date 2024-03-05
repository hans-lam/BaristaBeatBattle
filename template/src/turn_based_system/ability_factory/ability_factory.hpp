#pragma once

#include "../abilities.hpp"

class AbilityFactory {
public:
	Ability* construct_pour_it();
	Ability* construct_basic_attack();
	Ability* construct_rest();
};

extern AbilityFactory* ability_factory;

