#pragma once

#include <vector>
#include <map>
#include "character_system.hpp"
#include "abilities.hpp"
#include "../tiny_ecs_registry.hpp"
#include "../tiny_ecs.hpp"
#include "../ai_system.hpp"
#include "level_select_system.hpp"
#include "../world_init.hpp"
#include "experience.hpp"
#include "flag_progression.hpp"
#include <chrono>
#include <thread>
#include <cstdio>
#include <ctime>

class TurnBasedSystem {
	
public:

	TurnBasedSystem();

	void init(AISystem* ai_system);

	void step(float elapsed_ms_since_last_update);

	void start_encounter(Level* level);

	int process_character_action(Ability* ability, Character* caller, std::vector<Character*> recipients);

	Entity get_entity_given_character(Character* receiving_character);

	bool are_all_allies_defeated();
	bool are_all_enemies_defeated();

	void process_death(Entity o7);

	Entity get_active_character() {
		return active_character;
	}

	HealthBarFill& get_health_bar_given_entity(Entity receiving_character);

	std::clock_t end_of_game_wait = NULL;

private:

	Entity active_character = emptyEntity;
	
	bool waiting_for_player = false;
	
	AISystem* ai_system;

	int current_level;

	std::clock_t enemy_await = NULL;

};

extern bool out_of_combat;