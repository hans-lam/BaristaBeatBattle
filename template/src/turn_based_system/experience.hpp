#pragma once

#include "character_system.hpp"
#include "character_stat_sheet.hpp"


class ExperienceManager {
public:
	void apply_experience(Character* character_data, int experience_amount);
	void level_up_character(Character* character_data);
};


extern ExperienceManager experience_manager;

