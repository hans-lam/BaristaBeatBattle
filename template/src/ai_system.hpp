#pragma once

#include <vector>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "./turn_based_system/character_system.hpp"


class AISystem
{
public:
	void step(float elapsed_ms);

	Character* ai_find_target() {
		unsigned int lowest_health;
		Entity lowest_health_party_member = emptyEntity;
		Character* character_data;

		for (Entity party_member_entity : registry.partyMembers.entities) {
			character_data = registry.characterDatas.get(party_member_entity).characterData;
			unsigned int curr_health_points = character_data->get_current_health_points();

			if (!character_data->is_dead() && (lowest_health_party_member == emptyEntity || lowest_health > curr_health_points)) {
				lowest_health_party_member = party_member_entity;
				lowest_health = curr_health_points;
			}

		}

		return registry.characterDatas.get(lowest_health_party_member).characterData;
	}
};


