
#include "character_system.hpp"

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
}

unsigned int Character::get_current_health_points() {
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

CharacterStatSheet* Character::get_character_stat_sheet() {
	return this->stats;
}

