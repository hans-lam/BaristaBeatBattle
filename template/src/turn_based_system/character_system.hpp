#pragma once

#include "abilities.hpp"
#include "character_stat_sheet.hpp"
#include <vector>
#include <string>

// Have to forward declare Ability for some reason.
class Ability;

class Character {
public:
	
	Character(std::string name, CharacterStatSheet* stats);

	void add_ability(Ability* ability);

	void restore_health_to_full();

	unsigned int get_current_health_points();

	CharacterStatSheet* get_character_stat_sheet();

	bool is_dead();

	void deal_damage(unsigned int dmg) {
		unsigned int dmg_amount = std::max(current_health_points - dmg, (unsigned int)0);
		this->current_health_points -= dmg_amount;
	}

	void heal_amount(unsigned int heal) {
		unsigned int heal_amount = std::min(current_health_points + heal, stats->get_max_health());
		this->current_health_points += heal_amount;
	}

	std::string get_name();


private:
	std::string character_name;
	CharacterStatSheet* stats;
	std::vector<Ability*> abilities;

	unsigned int current_health_points;
};