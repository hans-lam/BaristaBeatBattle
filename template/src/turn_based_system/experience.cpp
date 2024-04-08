#pragma once

#include "experience.hpp"
#include "../world_init.hpp"
#include "common.hpp"

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
		LevelUpTimer& levelUp = registry.levelUpTimers.emplace(character_entity);
		Motion& caller_motion = registry.motions.get(character_entity);
		Entity level_text = createText("LEVEL UP!", vec2(caller_motion.position.x + 50.f, window_height_px - caller_motion.position.y), 0.7f, vec3(1.0f, 0.0f, 0.0f), mat4(1.0f), StageSystem::Stage::turn_based, false);
		registry.textRenderRequests.get(level_text).shown = true;
		levelUp.associated_text = level_text;
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