#include "character_factory.hpp"


Entity CharacterFactory::construct_chai(Entity entity) {
	std::string name = "Chai";
	unsigned int chai_max_health = 25;
	unsigned int chai_strength = 5;
	unsigned int chai_magic = 5;
	unsigned int chai_defense = 2;
	unsigned int chai_magic_resist = 2;
	unsigned int chai_speed = 16;

	CharacterStatSheet* character_stat = new CharacterStatSheet(chai_max_health, chai_strength, chai_magic, chai_defense, chai_magic_resist, chai_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();
	Ability* rest = ability_factory->construct_rest();
	

	Character* chai_character_obj = new Character(name, character_stat);
	chai_character_obj->add_ability(basic_attack);
	chai_character_obj->add_ability(rest);

	CharacterData characterData = CharacterData();
	characterData.characterData = chai_character_obj;
	registry.characterDatas.emplace(entity, characterData);
	

	return entity;

}

Entity CharacterFactory::construct_earl(Entity entity) {
	std::string name = "Earl";
	unsigned int earl_max_health = 45;
	unsigned int earl_strength = 8;
	unsigned int earl_magic = 2;
	unsigned int earl_defense = 3;
	unsigned int earl_magic_resist = 2;
	unsigned int earl_speed = 11;

	CharacterStatSheet* character_stat = new CharacterStatSheet(earl_max_health, earl_strength, earl_magic, earl_defense, earl_magic_resist, earl_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();
	Ability* rest = ability_factory->construct_rest();


	Character* earl_character_obj = new Character(name, character_stat);
	earl_character_obj->add_ability(basic_attack);
	earl_character_obj->add_ability(rest);

	CharacterData characterData = CharacterData();
	characterData.characterData = earl_character_obj;
	registry.characterDatas.emplace(entity, characterData);

	return entity;
}

Entity CharacterFactory::construct_americano(Entity entity) {
	std::string name = "Americano";
	unsigned int americano_max_health = 20;
	unsigned int americano_strength = 2;
	unsigned int americano_magic = 8;
	unsigned int americano_defense = 1;
	unsigned int americano_magic_resist = 3;
	unsigned int americano_speed = 9;

	CharacterStatSheet* character_stat = new CharacterStatSheet(americano_max_health, americano_strength, americano_magic, americano_defense, americano_magic_resist, americano_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();
	Ability* rest = ability_factory->construct_rest();


	Character* americano_character_obj = new Character(name, character_stat);
	americano_character_obj->add_ability(basic_attack);
	americano_character_obj->add_ability(rest);


	CharacterData characterData = CharacterData();
	characterData.characterData = americano_character_obj;
	registry.characterDatas.emplace(entity, characterData);

	return entity;
}

Entity CharacterFactory::construct_enemy(Entity entity, int level) {
	std::string name = "Enemy " + std::to_string(level);
	unsigned int enemy_max_health = 30 * level;
	unsigned int enemy_strength = 1 * level;
	unsigned int enemy_magic = 1 * level;
	unsigned int enemy_defense = 1 * level;
	unsigned int enemy_magic_resist = 1 * level;
	unsigned int enemy_speed = 2 * level;

	CharacterStatSheet* character_stat = new CharacterStatSheet(enemy_max_health, enemy_strength, enemy_magic, enemy_defense, enemy_magic_resist, enemy_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();


	Character* enemy_character_obj = new Character(name, character_stat);
	enemy_character_obj->add_ability(basic_attack);

	CharacterData characterData = CharacterData();
	characterData.characterData = enemy_character_obj;
	registry.characterDatas.emplace(entity, characterData);

	return entity;
}


CharacterFactory character_factory;