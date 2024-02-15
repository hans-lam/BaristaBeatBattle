#include "character_stat_sheet.hpp"


CharacterStatSheet::CharacterStatSheet(unsigned int max_health_ = 0,
	unsigned int strength_ = 0,
	unsigned int magic_ = 0,
	unsigned int defense_ = 0,
	unsigned int magic_resist_ = 0,
	unsigned int speed_ = 0) :
	max_health(max_health_),
	strength(strength_),
	magic(magic_),
	defense(defense_),
	magic_resist(magic_resist_),
	speed(speed_) {}

void CharacterStatSheet::set_max_health(unsigned int max_health_val) {
	max_health = max_health_val;
}
void CharacterStatSheet::set_strength(unsigned int strength_val) {
	strength = strength_val;
}
void CharacterStatSheet::set_magic(unsigned int magic_val) {
	magic = magic_val;
}
void CharacterStatSheet::set_defense(unsigned int defense_val) {
	defense = defense_val;
}
void CharacterStatSheet::set_magic_resist(unsigned int magic_resist_val) {
	magic_resist = magic_resist_val;
}
void CharacterStatSheet::set_speed(unsigned int speed_val) {
	speed = speed_val;
}


unsigned int CharacterStatSheet::get_max_health() {
	return max_health;
}
unsigned int CharacterStatSheet::get_strength() {
	return strength;
}
unsigned int CharacterStatSheet::get_magic() {
	return magic;
}
unsigned int CharacterStatSheet::get_defense() {
	return defense;
}
unsigned int CharacterStatSheet::get_magic_resist() {
	return magic_resist;
}
unsigned int CharacterStatSheet::get_speed() {
	return speed;
}