
#include "character_system.hpp"

// Character Class


Character::Character(std::string name, CharacterStatSheet stats) {
	this->character_name = name;
	this->stats = stats;
}



void Character::add_ability(Ability ability) {
	this->abilities.push_back(ability);
}

void Character::restore_health_to_full() {
	int max_health = stats.get_max_health();
	this->current_health_points = max_health;
}

int Character::get_current_health_points() {
	return this->current_health_points;
}

CharacterStatSheet Character::get_character_stat_sheet() {
	return this->stats;
}

CharacterStatSheet::CharacterStatSheet(int max_health, int strength, int magic, int defense, int magic_resist, int speed) {
	set_max_health(max_health);
	set_strength(strength);
	set_magic(magic);
	set_defense(defense);
	set_magic_resist(magic_resist);
	set_speed(speed);
}

void CharacterStatSheet::set_max_health(int max_health) {
	this->max_health = max_health;
}
void CharacterStatSheet::set_strength(int strength) {
	this->strength = strength;
}
void CharacterStatSheet::set_magic(int magic) {
	this->magic = magic;
}
void CharacterStatSheet::set_defense(int defense) {
	this->defense = defense;
}
void CharacterStatSheet::set_magic_resist(int magic_resist) {
	this->magic_resist = magic_resist;
}
void CharacterStatSheet::set_speed(int speed) {
	this->speed = speed;
}


int CharacterStatSheet::get_max_health() {
	return this->max_health;
}
int CharacterStatSheet::get_strength() {
	return this->strength;
}
int CharacterStatSheet::get_magic() {
	return this->magic;
}
int CharacterStatSheet::get_defense() {
	return this->defense;
}
int CharacterStatSheet::get_magic_resist() {
	return this->magic_resist;
}
int CharacterStatSheet::get_speed() {
	return this->);
}