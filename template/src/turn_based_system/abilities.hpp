#include "character_system.hpp"

#include <string>


class Ability{
public:
	Ability(int power, std::string name, bool is_physical, bool is_area_of_effect);

	int get_power();
	std::string get_ability_name();
	bool get_is_physical();
	bool get_is_area_of_effect();

	void process_ability(Character caller, Character callee);

private:
	int power;
	std::string ability_name;
	bool is_physical;
	bool is_area_of_effect;
};


#define PHYSCIAL_BASIC_ATTACK_POWER = 5;

Ability generic_basic_attack = Ability(PHYSCIAL_BASIC_ATTACK_POWER, "Basic Attack", true, false);