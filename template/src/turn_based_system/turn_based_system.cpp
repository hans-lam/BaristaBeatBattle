#include "turn_based_system.hpp"
#include "./character_factory/character_factory.hpp"


#include "common.hpp"
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <iostream>

const unsigned int SPEED_REQUIRED_FOR_TURN = 100;
const double HIT_CHANCE = 0.80;

TurnBasedSystem::TurnBasedSystem() {
	// stub;
	// influence for random code
	// https://www.geeksforgeeks.org/generate-a-random-number-between-0-and-1/
	srand(time(0));
}

void TurnBasedSystem::init() {
	construct_party();
}


void TurnBasedSystem::construct_party() {
	Entity chai = character_factory.construct_chai();
	Entity earl = character_factory.construct_earl();
	Entity americano = character_factory.construct_americano();
}

void TurnBasedSystem::step(float elapsed_ms_since_last_update) {

	if (out_of_combat || waiting_for_player) return;


	if (active_character == nullptr) {

		TurnCounter* highest = nullptr;
		Entity highest_entity_number;


		for (Entity turnCounterEntity : registry.turnCounter.entities) {

			TurnCounter* tc = registry.turnCounter.get(turnCounterEntity);

			tc->placement += tc->speed_value;

			if (highest == 0 || highest->placement < tc->placement) {
				highest = tc;
				highest_entity_number = turnCounterEntity;
			}

		}

		if (highest->placement < SPEED_REQUIRED_FOR_TURN) return;

		active_character = &highest_entity_number;
		highest->placement -= 100;

	}


	if (registry.partyMembers.has(*active_character)) {
		waiting_for_player = true;
		
	}
	else {
		// AI
		//process_character_action(generic_basic_attack, active_character->character, party_members);
	}
}


void TurnBasedSystem::start_encounter() {
	registry.turnCounter.clear();

	for (Entity partyMember : registry.partyMembers.entities) {
		Character* characterData = registry.characterDatas.get(partyMember).characterData;

		TurnCounter* turn = new TurnCounter();

		turn->placement = 0;
		turn->speed_value = characterData->get_character_stat_sheet()->get_speed();

		registry.turnCounter.emplace(partyMember, turn);

	}


	for (Entity enemyEntity : registry.turnBasedEnemies.entities) {

		Character* characterData = registry.characterDatas.get(enemyEntity).characterData;

		TurnCounter* turn = new TurnCounter();

		turn->placement = 0;
		turn->speed_value = characterData->get_character_stat_sheet()->get_speed();

		registry.turnCounter.emplace(enemyEntity, turn);

	}

	out_of_combat = false;
}

void TurnBasedSystem::process_character_action(Ability* ability) {
	//process_character_action(ability, active_character->character, current_enemies);
}

void TurnBasedSystem::process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients) {

	std::cout << "Current Character: " << caller->get_name() << '\n';
		
		//ability->process_ability(caller, receiving_character);
	double chance_hit = ((double)rand()) / RAND_MAX;
	if (chance_hit < HIT_CHANCE) {
		for (Character* receiving_character : recipients) {

			ability->process_ability(caller, receiving_character);


		}
	}

	TurnCounter* turn_counter = registry.turnCounter.get(*active_character);
	turn_counter->placement -= 100;

	active_character = nullptr;
	waiting_for_player = false;


	if (all_allies_defeated()) {
		out_of_combat = true;
		printf("Game Over! You won the fight!!");

		// TODO TRANSITION TO OVERWORLD WITH FIGHT GREYED OUT

	}

	if (all_enenmies_defeated()) {
		out_of_combat = true;
		printf("Game Over! You lost :(");

		// TODO TRANSTION TO OVERWORLD AND PLAYER HAS TO DO FIGHT AGAIN

	}

}


bool TurnBasedSystem::all_allies_defeated() {
	for (Entity partyMember : registry.partyMembers.entities) {
		Character* characterData = registry.characterDatas.get(partyMember).characterData;

		if (!characterData->is_dead()) {
			return false;
		}
	}
	return true;
}

bool TurnBasedSystem::all_enenmies_defeated() {
	for (Entity enemyEntity : registry.turnBasedEnemies.entities) {
		Character* characterData = registry.characterDatas.get(enemyEntity).characterData;

		if (!characterData->is_dead()) {
			return false;
		}
	}
	return true;
}



bool out_of_combat = true;