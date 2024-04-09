#include "character_factory.hpp"

Entity CharacterFactory::construct_chai(Entity entity) {
	std::string name = "Chai";

	// if character data already exists use that
	if (does_character_data_already_exist(entity, name)) {
		Character* character_data = registry.characterDatas.get(entity).characterData;
		Ability* basic_attack = ability_factory->construct_basic_attack();
		Ability* rest = ability_factory->construct_rest();
		Ability* pour = ability_factory->construct_pour_it();
		character_data->add_ability(basic_attack);
		character_data->add_ability(rest);
		character_data->add_ability(pour);
		return entity;
	}
	// else construct new data


	unsigned int chai_max_health = 25;
	unsigned int chai_strength = 5;
	unsigned int chai_magic = 5;
	unsigned int chai_defense = 2;
	unsigned int chai_magic_resist = 2;
	unsigned int chai_speed = 16;


	CharacterStatSheet* character_stat = new CharacterStatSheet(name, chai_max_health, chai_strength, chai_magic, chai_defense, chai_magic_resist, chai_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();
	Ability* rest = ability_factory->construct_rest();
	Ability* pour = ability_factory->construct_pour_it();
	

	Character* chai_character_obj = new Character(name, character_stat);
	chai_character_obj->add_ability(basic_attack);
	chai_character_obj->add_ability(rest);
	chai_character_obj->add_ability(pour);

	CharacterData characterData = CharacterData();
	characterData.characterData = chai_character_obj;
	registry.characterDatas.emplace(entity, characterData);	
	

	return entity;

}

Entity CharacterFactory::construct_earl(Entity entity) {
	std::string name = "Earl";

	// if character data already exists use that
	if (does_character_data_already_exist(entity, name)) {
		Character* character_data = registry.characterDatas.get(entity).characterData;
		Ability* basic_attack = ability_factory->construct_basic_attack();
		Ability* rest = ability_factory->construct_rest();
		Ability* pour = ability_factory->construct_pour_it();
		character_data->add_ability(basic_attack);
		character_data->add_ability(rest);
		character_data->add_ability(pour);
		return entity;
	}
	// else construct new data

	unsigned int earl_max_health = 20;
	unsigned int earl_strength = 2;
	unsigned int earl_magic = 8;
	unsigned int earl_defense = 2;
	unsigned int earl_magic_resist = 3;
	unsigned int earl_speed = 9;

	CharacterStatSheet* character_stat = new CharacterStatSheet(name, earl_max_health, earl_strength, earl_magic, earl_defense, earl_magic_resist, earl_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();
	Ability* rest = ability_factory->construct_rest();
	Ability* pour = ability_factory->construct_pour_it();



	Character* earl_character_obj = new Character(name, character_stat);
	earl_character_obj->add_ability(basic_attack);
	earl_character_obj->add_ability(rest);
	earl_character_obj->add_ability(pour);

	CharacterData characterData = CharacterData();
	characterData.characterData = earl_character_obj;
	registry.characterDatas.emplace(entity, characterData);

	return entity;
}

Entity CharacterFactory::construct_americano(Entity entity) {
	std::string name = "Americano";

	// if character data already exists use that
	if (does_character_data_already_exist(entity, name)) {
		Character* character_data = registry.characterDatas.get(entity).characterData;
		Ability* basic_attack = ability_factory->construct_basic_attack();
		Ability* rest = ability_factory->construct_rest();
		Ability* pour = ability_factory->construct_pour_it();
		character_data->add_ability(basic_attack);
		character_data->add_ability(rest);
		character_data->add_ability(pour);
		return entity;
	}
	// else construct new data

	unsigned int americano_max_health = 45;
	unsigned int americano_strength = 8;
	unsigned int americano_magic = 2;
	unsigned int americano_defense = 3;
	unsigned int americano_magic_resist = 1;
	unsigned int americano_speed = 11;

	CharacterStatSheet* character_stat = new CharacterStatSheet(name, americano_max_health, americano_strength, americano_magic, americano_defense, americano_magic_resist, americano_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();
	Ability* rest = ability_factory->construct_rest();
	Ability* pour = ability_factory->construct_pour_it();


	Character* americano_character_obj = new Character(name, character_stat);
	americano_character_obj->add_ability(basic_attack);
	americano_character_obj->add_ability(rest);
	americano_character_obj->add_ability(pour);


	CharacterData characterData = CharacterData();
	characterData.characterData = americano_character_obj;
	registry.characterDatas.emplace(entity, characterData);

	return entity;
}

Entity CharacterFactory::construct_london(Entity entity) {
	std::string name = "London";

	// if character data already exists use that
	if (does_character_data_already_exist(entity, name)) {
		Character* character_data = registry.characterDatas.get(entity).characterData;
		Ability* basic_attack = ability_factory->construct_basic_attack();
		Ability* rest = ability_factory->construct_rest();
		Ability* pour = ability_factory->construct_pour_it();
		character_data->add_ability(basic_attack);
		character_data->add_ability(rest);
		character_data->add_ability(pour);
		return entity;
	}
	// else construct new data

	unsigned int london_max_health = 18;
	unsigned int london_strength = 7;
	unsigned int london_magic = 4;
	unsigned int london_defense = 2;
	unsigned int london_magic_resist = 2;
	unsigned int london_speed = 15;

	CharacterStatSheet* character_stat = new CharacterStatSheet(name, london_max_health, london_strength, london_magic, london_defense, london_magic_resist, london_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();
	Ability* rest = ability_factory->construct_rest();
	Ability* pour = ability_factory->construct_pour_it();


	Character* london_character_obj = new Character(name, character_stat);
	london_character_obj->add_ability(basic_attack);
	london_character_obj->add_ability(rest);
	london_character_obj->add_ability(pour);


	CharacterData characterData = CharacterData();
	characterData.characterData = london_character_obj;
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

	CharacterStatSheet* character_stat = new CharacterStatSheet(name, enemy_max_health, enemy_strength, enemy_magic, enemy_defense, enemy_magic_resist, enemy_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();


	Character* enemy_character_obj = new Character(name, character_stat);
	enemy_character_obj->add_ability(basic_attack);
	enemy_character_obj->level = level * 2;

	CharacterData characterData = CharacterData();
	characterData.characterData = enemy_character_obj;
	registry.characterDatas.emplace(entity, characterData);

	return entity;
}

Entity CharacterFactory::construct_tutorial_enemy(Entity entity) {
	std::string name = "Tutorial Enemy";
	unsigned int enemy_max_health = 45;
	unsigned int enemy_strength = 15;
	unsigned int enemy_magic = 1;
	unsigned int enemy_defense = 1;
	unsigned int enemy_magic_resist = 8;
	unsigned int enemy_speed = 7;

	CharacterStatSheet* character_stat = new CharacterStatSheet(name, enemy_max_health, enemy_strength, enemy_magic, enemy_defense, enemy_magic_resist, enemy_speed);

	Ability* basic_attack = ability_factory->construct_basic_attack();


	Character* enemy_character_obj = new Character(name, character_stat);
	enemy_character_obj->add_ability(basic_attack);
	enemy_character_obj->level = 5;

	CharacterData characterData = CharacterData();
	characterData.characterData = enemy_character_obj;
	registry.characterDatas.emplace(entity, characterData);

	return entity;
}





int does_character_data_already_exist(Entity entity, std::string name) {
	// look to see if there is a stored character
	for (Entity stored_character_ent : registry.characterDatas.entities) {
		CharacterData curr_stored_character = registry.characterDatas.get(stored_character_ent);
		if (curr_stored_character.characterData->get_name() == name) {

			// store the character data with the new entity and remove the old
			registry.characterDatas.remove(stored_character_ent);
			
			registry.characterDatas.emplace(entity, curr_stored_character);
			return true;
		}
	}

	return false;
}

CharacterFactory* character_factory = new CharacterFactory();