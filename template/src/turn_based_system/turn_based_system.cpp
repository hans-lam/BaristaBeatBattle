#include "turn_based_system.hpp"
#include "./character_factory/character_factory.hpp"

void TurnBasedSystem::init() {
	construct_party();
}


void TurnBasedSystem::construct_party() {
	Character chai = character_factory.construct_chai();
	Character earl = character_factory.construct_earl();
	Character americano = character_factory.construct_americano();
	party_members.push_back(chai);
	party_members.push_back(earl);
	party_members.push_back(americano);
}

void TurnBasedSystem::step() {

}

void TurnBasedSystem::process_character_action(Ability ability, Character caller, vector<Character> recipients) {

}


void TurnBasedSystem::game_over() {

}