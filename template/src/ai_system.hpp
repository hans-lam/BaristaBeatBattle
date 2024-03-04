#pragma once

#include <vector>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"


// TODO Add more complex ai systems for Milestone 3
// current ai is in turn_based_system
//		ai selects basic attack
//		targets lowest health party memeber

class AISystem
{
public:
	void step(float elapsed_ms);
};