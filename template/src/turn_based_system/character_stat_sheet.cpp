#include "character_stat_sheet.hpp"


CharacterStatSheet::CharacterStatSheet(int max_health_ = 0,
	int strength_ = 0,
	int magic_ = 0,
	int defense_ = 0,
	int magic_resist_ = 0,
	int speed_ = 0) :
	max_health(max_health_),
	strength(strength_),
	magic(magic_),
	defense(defense_),
	magic_resist(magic_resist_),
	speed(speed_) {}

void CharacterStatSheet::set_max_health(int max_health_val) {
	max_health = max_health_val;
}
void CharacterStatSheet::set_strength(int strength_val) {
	strength = strength_val;
}
void CharacterStatSheet::set_magic(int magic_val) {
	magic = magic_val;
}
void CharacterStatSheet::set_defense(int defense_val) {
	defense = defense_val;
}
void CharacterStatSheet::set_magic_resist(int magic_resist_val) {
	magic_resist = magic_resist_val;
}
void CharacterStatSheet::set_speed(int speed_val) {
	speed = speed_val;
}


int CharacterStatSheet::get_max_health() {
	return max_health;
}
int CharacterStatSheet::get_strength() {
	return strength;
}
int CharacterStatSheet::get_magic() {
	return magic;
}
int CharacterStatSheet::get_defense() {
	return defense;
}
int CharacterStatSheet::get_magic_resist() {
	return magic_resist;
}
int CharacterStatSheet::get_speed() {
	return speed;
}