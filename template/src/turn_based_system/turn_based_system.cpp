#include "turn_based_system.hpp"
#include "./character_factory/character_factory.hpp"

TurnBasedSystem::TurnBasedSystem() {
	// stub;

}

void TurnBasedSystem::init() {
	construct_party();
}


void TurnBasedSystem::construct_party() {
	Character* chai = character_factory.construct_chai();
	Character* earl = character_factory.construct_earl();
	Character* americano = character_factory.construct_americano();
	party_members.push_back(chai);
	party_members.push_back(earl);
	party_members.push_back(americano);
}

void TurnBasedSystem::step() {

}


void TurnBasedSystem::start_encounter(std::vector<Character*> enemies) {
	this->current_enemies.clear();
	this->current_enemies = enemies;

	this->turn_counter_list.clear();

	for (Character * character : party_members) {

		TurnCounter turn;

		turn.character = character;
		turn.placement = 0;
		turn.speed_value = character->get_character_stat_sheet()->get_speed();

	}


	for (Character* character : current_enemies) {

		TurnCounter turn;

		turn.character = character;
		turn.placement = 0;
		turn.speed_value = character->get_character_stat_sheet()->get_speed();

	}
}

void TurnBasedSystem::process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients) {
	// stub
}


bool TurnBasedSystem::is_game_over() {
	for (Character* character : party_members) {
		if (!character->is_dead()) {
			return false;
		}
	}
	return true;
}