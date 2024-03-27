#pragma once

#include <vector>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "./turn_based_system/character_system.hpp"



class AISystem
{
public:
	void step(float elapsed_ms);

	// find the character with the lowest hp that was not the previous target
	Character* ai_find_target() {
		unsigned int lowest_health;
		Entity lowest_health_party_member = emptyEntity;
		Character* character_data;
		int number_of_party_members = registry.partyMembers.size();

		for (Entity party_member_entity : registry.partyMembers.entities) {
			character_data = registry.characterDatas.get(party_member_entity).characterData;
			unsigned int curr_health_points = character_data->get_current_health_points();


			// character must be alive
			// EITHER
			//   It does not target the same character twice
			//	 Or there is only one ally left
			// it is less hp then the current lowest target
			if (!character_data->is_dead() && (number_of_party_members == 1 || (character_data != last_targeted_character)) && (lowest_health_party_member == emptyEntity || lowest_health > curr_health_points)) {
				lowest_health_party_member = party_member_entity;
				lowest_health = curr_health_points;
			}

		}

		Character* target_character = registry.characterDatas.get(lowest_health_party_member).characterData;
		last_targeted_character = target_character;
		return target_character;
	}

private:
	Character* last_targeted_character;
};


