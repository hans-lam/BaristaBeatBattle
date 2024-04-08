#pragma once

#include "experience.hpp"

const unsigned int EXPERIENCE_REQUIRED_TO_LEVEL = 5;

void ExperienceManager::apply_experience(Entity character_entity, int experience_amount) {
	Character* character_data = registry.characterDatas.get(character_entity).characterData;

	character_data->curr_experience_points += experience_amount;

	if (character_data->curr_experience_points >= EXPERIENCE_REQUIRED_TO_LEVEL) {
		level_up_character(character_entity);
	}

}

void ExperienceManager::level_up_character(Entity character_entity) {
	if (!registry.levelUpTimers.has(character_entity)) {
		registry.levelUpTimers.emplace(character_entity);
	}

	Character* character_data = registry.characterDatas.get(character_entity).characterData;

	character_data->curr_experience_points -= EXPERIENCE_REQUIRED_TO_LEVEL;
	character_data->level++;

	CharacterStatSheet* stats = character_data->get_character_stat_sheet();

	stats->set_max_health(stats->get_max_health() + 5);
	stats->set_strength(stats->get_strength() + 1);
	stats->set_magic(stats->get_magic() + 1);
	stats->set_defense(stats->get_defense() + 1);
	stats->set_magic_resist(stats->get_magic_resist() + 1);

}

ExperienceManager experience_manager;