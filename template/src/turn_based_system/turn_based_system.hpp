#pragma once

#include <vector>
#include <map>
#include "character_system.hpp"
#include "abilities.hpp"


struct TurnCounter;

class TurnBasedSystem {
public:

	TurnBasedSystem();

	void init();

	void step();

	void start_encounter(std::vector<Character*> enemies);

	void process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients);

	bool is_game_over();

private:
	std::vector<Character*> party_members;

	std::vector<Character*> current_enemies;

	void construct_party();

	Character* active_character = nullptr;

	std::vector<TurnCounter> turn_counter_list;
};


struct TurnCounter {
	Character * character;
	int placement;
	int speed_value;
};