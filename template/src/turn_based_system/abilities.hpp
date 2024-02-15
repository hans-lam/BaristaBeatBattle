#pragma once

#include "character_system.hpp"
#include <vector>
#include <string>


class Ability{
public:
	Ability(unsigned int power, std::string name, bool is_physical, bool is_area_of_effect);

	unsigned int get_power();
	std::string get_ability_name();
	bool get_is_physical();
	bool get_is_area_of_effect();

	//void process_ability(Character* caller, Character* callee);

private:
	int power;
	std::string ability_name;
	bool is_physical;
	bool is_area_of_effect;
};


const int PHYSCIAL_BASIC_ATTACK_POWER = 5;

extern Ability* generic_basic_attack;