#pragma once

#include "abilities.hpp"
#include "character_stat_sheet.hpp"
#include <vector>
#include <string>
#include <algorithm>

// Have to forward declare Ability for some reason.
class Ability;

class Character {
public:

	Character(std::string name, CharacterStatSheet* stats);

	void add_ability(Ability* ability);

	void restore_health_to_full();

	int get_current_health_points();

	CharacterStatSheet* get_character_stat_sheet();

	bool is_dead();

	void deal_damage(int dmg);

	void heal_amount(int heal);

	std::string get_name();

	Ability* get_ability_by_name(std::string name);


private:
	std::string character_name;
	CharacterStatSheet* stats;
	std::vector<Ability*> abilities;

	int current_health_points;
};	