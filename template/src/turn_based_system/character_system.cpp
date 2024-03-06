
#include "character_system.hpp"
#include <iostream>

// Character Class


Character::Character(std::string name, CharacterStatSheet* stats) {
	this->character_name = name;
	this->stats = stats;
	this->current_health_points = stats->get_max_health();
}

void Character::add_ability(Ability* ability) {
	this->abilities.push_back(ability);
}

void Character::restore_health_to_full() {
	int max_health = stats->get_max_health();
	this->current_health_points = max_health;

	std::cout << this->character_name << " Is now full HP!   " << this->current_health_points << "/" << this->current_health_points << '\n';
}

int Character::get_current_health_points() {
	return this->current_health_points;
}

std::string Character::get_name() {
	return this->character_name;
}

bool Character::is_dead() {
	if (current_health_points <= 0) {
		return true;
	}
	return false;
}


void Character::deal_damage(int dmg) {
	int new_health_amount = std::max(current_health_points - dmg, 0);
	this->current_health_points = new_health_amount;
	std::cout << this->character_name << " is now " << this->current_health_points << " health points" << '\n';
}

void Character::heal_amount(int heal) {
	int heal_amount = std::min(current_health_points + heal, stats->get_max_health());
	this->current_health_points = heal_amount;
	std::cout << this->character_name << " is now " << this->current_health_points << " health points" << '\n';
}

Ability* Character::get_ability_by_name(std::string name) {

	for (Ability* ability : abilities) {
		if (ability->get_ability_name() == name) {
			return ability;
		}
	}

	return nullptr;
};


CharacterStatSheet* Character::get_character_stat_sheet() {
	return this->stats;
}
