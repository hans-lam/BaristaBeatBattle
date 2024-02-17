#include "character_factory.hpp"


Character* CharacterFactory::construct_chai() {
	std::string name = "Chai";
	unsigned int chai_max_health = 25;
	unsigned int chai_strength = 10;
	unsigned int chai_magic = 10;
	unsigned int chai_defense = 10;
	unsigned int chai_magic_resist = 10;
	unsigned int chai_speed = 10;

	CharacterStatSheet* character_stat = new CharacterStatSheet(chai_max_health, chai_strength, chai_magic, chai_defense, chai_magic_resist, chai_speed);

	Ability* basic_attack = generic_basic_attack;
	

	Character* chai_character_obj = new Character(name, character_stat);
	chai_character_obj->add_ability(basic_attack);

	return chai_character_obj;

}

Character* CharacterFactory::construct_earl() {
	std::string name = "Earl";
	unsigned int earl_max_health = 10;
	unsigned int earl_strength = 10;
	unsigned int earl_magic = 10;
	unsigned int earl_defense = 10;
	unsigned int earl_magic_resist = 10;
	unsigned int earl_speed = 10;

	CharacterStatSheet* character_stat = new CharacterStatSheet(earl_max_health, earl_strength, earl_magic, earl_defense, earl_magic_resist, earl_speed);

	Ability* basic_attack = generic_basic_attack;


	Character* earl_character_obj = new Character(name, character_stat);
	earl_character_obj->add_ability(basic_attack);

	return earl_character_obj;
}

Character* CharacterFactory::construct_americano() {
	std::string name = "Americano";
	unsigned int americano_max_health = 10;
	unsigned int americano_strength = 10;
	unsigned int americano_magic = 10;
	unsigned int americano_defense = 10;
	unsigned int americano_magic_resist = 10;
	unsigned int americano_speed = 10;

	CharacterStatSheet* character_stat = new CharacterStatSheet(americano_max_health, americano_strength, americano_magic, americano_defense, americano_magic_resist, americano_speed);

	Ability* basic_attack = generic_basic_attack;


	Character* americano_character_obj = new Character(name, character_stat);
	americano_character_obj->add_ability(basic_attack);

	return americano_character_obj;
}

Character* CharacterFactory::construct_enemy(int level) {
	std::string name = "Enemy " + std::to_string(level);
	unsigned int enemy_max_health = 10 * level;
	unsigned int enemy_strength = 1 * level;
	unsigned int enemy_magic = 1 * level;
	unsigned int enemy_defense = 1 * level;
	unsigned int enemy_magic_resist = 1 * level;
	unsigned int enemy_speed = 1 * level;

	CharacterStatSheet* character_stat = new CharacterStatSheet(enemy_max_health, enemy_strength, enemy_magic, enemy_defense, enemy_magic_resist, enemy_speed);

	Ability* basic_attack = generic_basic_attack;


	Character* enemy_character_obj = new Character(name, character_stat);
	enemy_character_obj->add_ability(basic_attack);

	return enemy_character_obj;
}


CharacterFactory character_factory;