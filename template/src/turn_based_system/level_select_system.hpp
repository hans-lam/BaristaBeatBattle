#pragma once

#include "../tiny_ecs.hpp"
#include <vector>

class Level {
public:

	std::vector<Entity> allies;
	std::vector<Entity> enemies;
};