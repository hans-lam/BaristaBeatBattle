#pragma once

#include <vector>
#include <map>
#include "character_system.hpp"
#include "abilities.hpp"
#include "../tiny_ecs_registry.hpp"
#include "../tiny_ecs.hpp"


class TurnBasedSystem {
	
public:

	TurnBasedSystem();

	void init();

	void step(float elapsed_ms_since_last_update);

	void start_encounter();

	void process_character_action(Ability* ability);
	void process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients);

	bool all_allies_defeated();
	bool all_enenmies_defeated();

	Entity get_active_character() {
		return active_character;
	}

private:

	void construct_party();

	Entity active_character = emptyEntity;
	
	bool waiting_for_player = false;

};

extern bool out_of_combat;