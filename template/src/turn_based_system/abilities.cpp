
#include "abilities.hpp"

Ability::Ability(unsigned int power, std::string ability_name, bool is_physical, bool is_area_of_effect) {
	this->power = power;
	this->ability_name = ability_name;
	this->is_physical = is_physical;
	this->is_area_of_effect = is_area_of_effect;
}

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

void Ability::process_ability(Character* caller, Character* target) {
	CharacterStatSheet* caller_stat_sheet = caller->get_character_stat_sheet();
	CharacterStatSheet* target_stat_sheet = target->get_character_stat_sheet();

	unsigned int attack_amount;
	unsigned int defense_amount;

	if (get_is_physical()) {
		attack_amount = get_power() + caller_stat_sheet->get_strength();
		defense_amount = target_stat_sheet->get_defense();
	}
	else {
		attack_amount = get_power() + caller_stat_sheet->get_magic();
		defense_amount = target_stat_sheet->get_magic_resist();
	}

	unsigned int damage_amount = std::max(attack_amount - defense_amount, (unsigned int)0);

	target->deal_damage(damage_amount);
}


SupportAbility::SupportAbility(unsigned int potency, std::string ability_name, bool is_healing, bool is_area_of_effect) : Ability(0, ability_name, false, is_area_of_effect) {
	this->potency = potency;
	this->is_healing = is_healing;
}

unsigned int SupportAbility::get_potency() {
	return this->potency;
}

bool SupportAbility::get_is_healing() {
	return this->is_healing;
}

void SupportAbility::process_ability(Character* caller, Character* target) {
	CharacterStatSheet* caller_stat_sheet = caller->get_character_stat_sheet();
	CharacterStatSheet* target_stat_sheet = target->get_character_stat_sheet();

	unsigned int heal_amount = get_potency() + caller_stat_sheet->get_magic();

	target->heal_amount(heal_amount);
}
