#include "turn_based_system.hpp"
#include "./character_factory/character_factory.hpp"

#include "common.hpp"
#include <algorithm>

const  unsigned int SPEED_REQUIRED_FOR_TURN = 100;

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

void TurnBasedSystem::step(float elapsed_ms_since_last_update) {

	if (out_of_combat || waiting_for_player) return;


	if (active_character == nullptr) {

		TurnCounter* highest = nullptr;


		for (TurnCounter* tc : turn_counter_list) {

			tc->placement += tc->speed_value;

			if (highest == nullptr || highest->placement < tc->placement) {
				highest = tc;
			}

		}

		if (highest->placement < SPEED_REQUIRED_FOR_TURN) return;

		active_character = highest;
		highest->placement -= 100;

	}


	if (std::find(current_enemies.begin(), current_enemies.end(), active_character->character) != current_enemies.end()) {

		process_character_action(generic_basic_attack, active_character->character, party_members);

	}
	else {
		waiting_for_player = true;
	}


}


void TurnBasedSystem::start_encounter(std::vector<Character*> enemies) {
	this->current_enemies.clear();
	this->current_enemies = enemies;

	this->turn_counter_list.clear();

	for (Character * character : party_members) {

		TurnCounter* turn = new TurnCounter();

		turn->character = character;
		turn->placement = 0;
		turn->speed_value = character->get_character_stat_sheet()->get_speed();

		turn_counter_list.push_back(turn);

	}


	for (Character* character : current_enemies) {

		TurnCounter* turn = new TurnCounter();

		turn->character = character;
		turn->placement = 0;
		turn->speed_value = character->get_character_stat_sheet()->get_speed();

		turn_counter_list.push_back(turn);

	}

	out_of_combat = false;
}

void TurnBasedSystem::process_character_action(Ability* ability) {
	process_character_action(ability, active_character->character, current_enemies);
}

void TurnBasedSystem::process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients) {
	// stub

	unsigned int attack_amount = ability->get_power() + caller->get_character_stat_sheet()->get_strength();

	for (Character* receiving_character : recipients) {

		unsigned int defense_amount = receiving_character->get_character_stat_sheet()->get_defense();


		unsigned int damage_amount = std::max(attack_amount - defense_amount, (unsigned int) 0);

		receiving_character->deal_damage(damage_amount);


	}


	active_character->placement -= 100;
	active_character = nullptr;
	waiting_for_player = false;


	if (is_game_over()) {
		out_of_combat = true;
	}

}


bool TurnBasedSystem::is_game_over() {
	for (Character* character : party_members) {
		if (!character->is_dead()) {
			return false;
		}
	}
	return true;
}


TurnCounter::TurnCounter() {
	placement = 0;
}


bool out_of_combat = true;