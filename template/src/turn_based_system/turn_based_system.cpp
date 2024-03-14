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

void TurnBasedSystem::init(AISystem* ai_system) {
	this->ai_system = ai_system;
}

void TurnBasedSystem::step(float elapsed_ms_since_last_update) {

	if (out_of_combat || waiting_for_player) return;

	if (active_character == emptyEntity) {

		TurnCounter* highest = NULL;
		Entity highest_entity_number;

		for (Entity turnCounterEntity : registry.turnCounter.entities) {

			TurnCounter* tc = registry.turnCounter.get(turnCounterEntity);

			tc->placement += tc->speed_value;

			if (highest == NULL || highest->placement < tc->placement) {
				highest = tc;
				highest_entity_number = turnCounterEntity;
			}

		}

		assert(highest != NULL && "There ara no character in the turn counter");

		if (highest->placement < SPEED_REQUIRED_FOR_TURN) return;

		active_character = highest_entity_number;
		highest->placement -= 100;

	}


	if (registry.partyMembers.has(active_character)) {
		waiting_for_player = true;

	}
	else {

		Character* ai_character = registry.characterDatas.get(active_character).characterData;


		Character* target_character = ai_system->ai_find_target();

		std::cout << ai_character->get_name() << "'s targeting " << target_character->get_name() << " best they have the lowest health!" << "\n";

		process_character_action(ai_character->get_ability_by_name("Basic Attack"), ai_character, { target_character});
	}
}


void TurnBasedSystem::start_encounter(Level* level) {
	registry.partyMembers.clear();
	registry.turnBasedEnemies.clear();
	registry.turnCounter.clear();


	for (Entity ally_entity : level->allies) {
		PartyMember partyMemberComponent = PartyMember();
		registry.partyMembers.emplace(ally_entity, partyMemberComponent);

		Character* characterData = registry.characterDatas.get(ally_entity).characterData;

		TurnCounter* turn = new TurnCounter();

		turn->placement = 0;
		turn->speed_value = characterData->get_character_stat_sheet()->get_speed();

		registry.turnCounter.emplace(ally_entity, turn);
	}

	for (Entity enemy_entity : level->enemies) {
		TurnBasedEnemy tbe = TurnBasedEnemy();
		registry.turnBasedEnemies.emplace(enemy_entity, tbe);

		Character* characterData = registry.characterDatas.get(enemy_entity).characterData;

		TurnCounter* turn = new TurnCounter();

		turn->placement = 0;
		turn->speed_value = characterData->get_character_stat_sheet()->get_speed();

		registry.turnCounter.emplace(enemy_entity, turn);
	}


	out_of_combat = false;
}

/*

0 = normal return state
1 = allies win
2 = enemies win

*/

int TurnBasedSystem::process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients) {

	std::cout << "Current Character: " << caller->get_name() << '\n';

		//ability->process_ability(caller, receiving_character);
	double chance_hit = ((double)rand()) / RAND_MAX;
	if (chance_hit < HIT_CHANCE) {
		for (Character* receiving_character : recipients) {

			ability->process_ability(caller, receiving_character);

			if (receiving_character->is_dead()) {


				process_death(get_entity_given_character(receiving_character));


			}

		}
	}
	else {
		std::cout << caller->get_name() << " Missed!" << '\n';
	}

	active_character = emptyEntity;
	waiting_for_player = false;


	if (are_all_allies_defeated()) {
		out_of_combat = true;
		printf("Game Over! You lost :(");


		return 2;

	}

	if (are_all_enemies_defeated()) {
		out_of_combat = true;
		printf("Game Over! You won the fight!!");

		return 1;
	}

	return 0;
}




bool TurnBasedSystem::are_all_allies_defeated() {
	for (Entity partyMember : registry.partyMembers.entities) {
		Character* characterData = registry.characterDatas.get(partyMember).characterData;

		if (!characterData->is_dead()) {
			return false;
		}
	}
	return true;
}

bool TurnBasedSystem::are_all_enemies_defeated() {
	for (Entity enemyEntity : registry.turnBasedEnemies.entities) {
		Character* characterData = registry.characterDatas.get(enemyEntity).characterData;

		if (!characterData->is_dead()) {
			return false;
		}
	}
	return true;
}

void TurnBasedSystem::process_death(Entity o7)
{

	if (registry.turnBasedEnemies.has(o7)) {
		registry.turnBasedEnemies.remove(o7);
	}

	registry.turnCounter.remove(o7);

}

Entity TurnBasedSystem::get_entity_given_character(Character* receiving_character)
{

	for (int i = 0; i < registry.characterDatas.size(); i++) {

		if (registry.characterDatas.components.at(i).characterData == receiving_character) {
			return registry.characterDatas.entities.at(i);
		}

	}
	return emptyEntity;
}


bool out_of_combat = true;