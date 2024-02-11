#include "character_factory.hpp"


Character CharacterFactory::construct_chai() {
	std::string name = "Chai";
	int chai_max_health = 10;
	int chai_strength = 1;
	int chai_magic = 1;
	int chai_defense = 1;
	int chai_magic_resist = 1;
	int chai_speed = 1;

	CharacterStatSheet character_stat = CharacterStatSheet(chai_max_health, chai_strength, chai_magic, chai_defense, chai_magic_resist, chai_speed);

	Ability basic_attack = generic_basic_attack;
	

	Character chai_character_obj = Character(name, character_stat);
	chai_character_obj.add_ability(basic_attack);

}

Character CharacterFactory::construct_earl() {
	std::string name = "Earl";
	int earl_max_health = 10;
	int earl_strength = 1;
	int earl_magic = 1;
	int earl_defense = 1;
	int earl_magic_resist = 1;
	int earl_speed = 1;

	CharacterStatSheet character_stat = CharacterStatSheet(earl_max_health, earl_strength, earl_magic, earl_defense, earl_magic_resist, earl_speed);

	Ability basic_attack = generic_basic_attack;


	Character chai_character_obj = Character(name, character_stat);
	chai_character_obj.add_ability(basic_attack);

}

Character CharacterFactory::construct_americano() {
	std::string name = "Americano";
	int americano_max_health = 10;
	int americano_strength = 1;
	int americano_magic = 1;
	int americano_defense = 1;
	int americano_magic_resist = 1;
	int americano_speed = 1;

	CharacterStatSheet character_stat = CharacterStatSheet(americano_max_health, americano_strength, americano_magic, americano_defense, americano_magic_resist, americano_speed);

	Ability basic_attack = generic_basic_attack;


	Character chai_character_obj = Character(name, character_stat);
	chai_character_obj.add_ability(basic_attack);
}

Character CharacterFactory::construct_enemy(int level) {
	std::string name = "Level " + level + " Ground";
	int enemy_max_health = 10 * level;
	int enemy_strength = 1 * level;
	int enemy_magic = 1 * level;
	int enemy_defense = 1 * level;
	int enemy_magic_resist = 1 * level;
	int enemy_speed = 1 * level;

	CharacterStatSheet character_stat = CharacterStatSheet(enemy_max_health, enemy_strength, enemy_magic, enemy_defense, enemy_magic_resist, enemy_speed);

	Ability basic_attack = generic_basic_attack;


	Character chai_character_obj = Character(name, character_stat);
	chai_character_obj.add_ability(basic_attack);

}