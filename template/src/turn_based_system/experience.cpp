#pragma once

#include "experience.hpp"

const unsigned int EXPERIENCE_REQUIRED_TO_LEVEL = 5;

void ExperienceManager::apply_experience(Character* character_data, int experience_amount) {

	character_data->curr_experience_points += experience_amount;

	if (character_data->curr_experience_points >= EXPERIENCE_REQUIRED_TO_LEVEL) {
		level_up_character(character_data);
	}

}

void ExperienceManager::level_up_character(Character* character_data) {

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