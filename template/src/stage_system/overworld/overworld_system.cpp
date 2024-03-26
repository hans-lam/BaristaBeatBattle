#include "overworld_system.hpp"
#include "tiny_ecs_registry.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <world_init.hpp>

OverworldSystem::OverworldSystem() : 
	overworld_tutorial(false)
{
}

void OverworldSystem::init(StageSystem* stage_system_arg) {
	stage_system = stage_system_arg;
	overworld_tutorial = false;
	// I dont think this call works
	//create_overworld_levels();
} 

void OverworldSystem::handle_overworld_keys(int key, int action, float player_speed) {
	// Movement keys
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT || key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
		handle_player_movement(key, action, player_speed);
	} 
	// Attack key
	else if (action == GLFW_PRESS && key == GLFW_KEY_A) {
		player_attack();
	}
	// Tutorial key
	else if (action == GLFW_PRESS && key == GLFW_KEY_T) {
		handle_tutorial();
	}
}

//void OverworldSystem::create_overworld_levels() {
//	vec2 level1pos = vec2(100, 100);
//	Entity test = createEnemyDrink(renderer, vec2(0.f, 0.f), level1pos);
//	std::cout << "i wanna make sure this is not being run on a loop or else that would be bad" << std::endl;
//}

void OverworldSystem::handle_player_movement(int key, int action, float player_speed) {
	for (int i = 0; i < registry.players.size(); i++) {
		Motion& player_motion = registry.motions.get(registry.players.entities[i]);
		// do not move player if it is dying
		if (registry.deathTimers.has(registry.players.entities[i])) continue;
		// check if player is in bounds
		bool is_x = (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT);
		//if (!player_in_bounds(&player_motion, is_x)) {
		//	action = GLFW_RELEASE;
		//}

		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			if (key == GLFW_KEY_UP) {
				player_motion.velocity.y = -player_speed;
			}
			else if (key == GLFW_KEY_LEFT) {
				//player_motion.velocity.x = -player_speed;

			}
			else if (key == GLFW_KEY_DOWN) {
				player_motion.velocity.y = player_speed;
			}
			else if (key == GLFW_KEY_RIGHT) {
				player_motion.velocity.x = player_speed;
			}

		}
		else if (action == GLFW_RELEASE) {
			if (key == GLFW_KEY_UP) {
				player_motion.velocity.y = 0.0f;
			}
			else if (key == GLFW_KEY_LEFT) {
				player_motion.velocity.x = 0.0f;
			}
			else if (key == GLFW_KEY_DOWN) {
				player_motion.velocity.y = 0.0f;
			}
			else if (key == GLFW_KEY_RIGHT) {
				player_motion.velocity.x = 0.0f;
			}
		}
	}
}

void OverworldSystem::player_attack() {
	for (uint i = 0; i < registry.players.size(); i++) {
		Entity entity = registry.players.entities[i];
		if (!registry.attackTimers.has(entity)) {
			registry.attackTimers.emplace(registry.players.entities[i]);
		}
	}
}

void OverworldSystem::handle_tutorial() {
	for (Entity entity : registry.tutorials.entities) {
		if (registry.overWorld.has(entity)) {
			RenderRequest& rr = registry.renderRequests.get(entity);

			if (overworld_tutorial) {
				// set the shown to false for the tutorial board
				rr.shown = false;
			}
			else {
				rr.shown = true;
			}

			overworld_tutorial = !overworld_tutorial;
			break;
		}
	}
}
