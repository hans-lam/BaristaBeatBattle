#pragma once

#include "../tiny_ecs.hpp"
#include <vector>

class Level {
public:
	int level_number;
	std::vector<Entity> allies;
	std::vector<Entity> enemies;
};