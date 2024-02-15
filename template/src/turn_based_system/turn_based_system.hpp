#pragma once

#include <vector>
#include <map>
#include "character_system.hpp"
#include "abilities.hpp"


class TurnCounter;

class TurnBasedSystem {
	
public:

	TurnBasedSystem();

	void init();

	void step(float elapsed_ms_since_last_update);

	void start_encounter(std::vector<Character*> enemies);

	void process_character_action(Ability* ability);
	void process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients);

	bool is_game_over();

private:
	

	std::vector<Character*> party_members;

	std::vector<Character*> current_enemies;

	void construct_party();

	TurnCounter* active_character = nullptr;

	std::vector<TurnCounter*> turn_counter_list;

	bool waiting_for_player = false;

};


class TurnCounter {
public:
	TurnCounter();

	Character * character;
	int placement = 0;
	int speed_value = 0;
};

extern bool out_of_combat;