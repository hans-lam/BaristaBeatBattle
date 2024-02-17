
#include "abilities.hpp"

Ability::Ability(unsigned int power, std::string ability_name, bool is_physical, bool is_area_of_effect) {
	this->power = power;
	this->ability_name = ability_name;
	this->is_physical = is_physical;
	this->is_area_of_effect = is_area_of_effect;
}

/*
void Ability::process_ability(Character* caller, Character* recipient) {
	CharacterStatSheet caller_stat_sheet = caller->get_character_stat_sheet();
	CharacterStatSheet recipient_stat_sheet = recipient->get_character_stat_sheet();
}
*/


unsigned int Ability::get_power() {
	return this->power;
}
std::string Ability::get_ability_name() {
	return this->ability_name;
}
bool Ability::get_is_physical() {
	return this->is_physical;
}
bool Ability::get_is_area_of_effect() {
	return this->is_area_of_effect;
}


Ability* generic_basic_attack = new Ability(PHYSCIAL_BASIC_ATTACK_POWER, "Basic Attack", true, false);