#pragma once

#include "character_system.hpp"
#include "character_stat_sheet.hpp"
#include "tiny_ecs_registry.hpp"


class ExperienceManager {
public:
	void apply_experience(Entity character_entity, int experience_amount);
	void level_up_character(Entity character_entity);
};


extern ExperienceManager experience_manager;

