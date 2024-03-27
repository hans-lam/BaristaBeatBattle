#include "overworld_system.hpp"
#include "tiny_ecs_registry.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <world_init.hpp>
#include <thread>

// Justin: NOTE THAT SOME OF THE FUNCTOINALITY IS ALSO IN world_system.cpp


OverworldSystem::OverworldSystem() : 
	overworld_tutorial(false)
{
}

void OverworldSystem::init(StageSystem* stage_system_arg) {
	stage_system = stage_system_arg;
	overworld_tutorial = false;
	// I dont think this call works
	//create_overworld_levels();
	std::cout << "IS THIS INIT BEING CALLED?????" << std::endl;
	
			
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

	// used to check when to set velocities to 0;
	float remaining_distance_x = 0.f;
	
	float remaining_distance_y = 0.f;

	for (int i = 0; i < registry.players.size(); i++) {
		Motion& player_motion = registry.motions.get(registry.players.entities[i]);
		// do not move player if it is dying
		if (registry.deathTimers.has(registry.players.entities[i])) continue;
		// check if player is in bounds
		bool is_x = (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT);
		//if (!player_in_bounds(&player_motion, is_x)) {
		//	action = GLFW_RELEASE;
		//}
		float nearest_left_dist = std::numeric_limits<float>::max();
		float nearest_right_dist = std::numeric_limits<float>::max();

		// Make every key press find the nearest level and then just go there. If left or right, prioritize x nearness. if up or down, prioritize y nearness, If tie, prioritize x.
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {

			float leftmost_x = std::numeric_limits<float>::max();
			float rightmost_x = std::numeric_limits<float>::lowest();

			for (LevelNode& levelnode : registry.levelNode.components) {
				if (levelnode.position.x < leftmost_x) {
					leftmost_x = levelnode.position.x;
				}
				if (levelnode.position.x > rightmost_x) {
					rightmost_x = levelnode.position.x;
				}
			}

			// Initialize variables to track the nearest nodes.
			LevelNode nearest_left_node;
			LevelNode nearest_right_node;
			bool found_left_node = false;
			bool found_right_node = false;

			for (LevelNode& levelnode : registry.levelNode.components) {

				// Calculate distance from player to current node.
				float dist = sqrt(pow((levelnode.position.x - player_motion.position.x), 2) + pow((levelnode.position.y - player_motion.position.y), 2));

				// Check if this node is to the left of the player.
				if (levelnode.position.x < player_motion.position.x && dist < nearest_left_dist) {
					nearest_left_dist = dist;
					nearest_left_node = levelnode;
					found_left_node = true;
				}

				// Check if this node is to the right of the player.
				if (levelnode.position.x > player_motion.position.x && dist < nearest_right_dist) {
					nearest_right_dist = dist;
					nearest_right_node = levelnode;
					found_right_node = true;
				}
			}
			
			//// Go through all levelNodes:
			//vec2 nearest_levelNode_pos = vec2(0.f, 0.f);
			//float curr_best_dist = 9999.f;
			//LevelNode nearest_left_node = LevelNode();
			//LevelNode nearest_right_node = LevelNode();
			//for (LevelNode levelnode : registry.levelNode.components) {
			//	
			//	// if find closer node, replace nearest_levelNode_pos with new best
			//	vec2 nodePos = levelnode.position;
			//	
			//	// find nearest_left_node means that we check if levelnode.position.x is less than the player.position.x
			//	if (levelnode.position.x > player_motion.position.x) {
			//		
			//		// dist to left calculation
			//		float dist = sqrt(pow((levelnode.position.x - player_motion.position.x), 2) + pow(levelnode.position.y - player_motion.position.y, 2));
			//		if (dist < curr_best_dist) {
			//			curr_best_dist = dist;
			//			nearest_levelNode_pos = levelnode.position;
			//			nearest_right_node = levelnode;
			//			std::cout << "THIS IS THE POSITION OF THE nearest_right_node: (x, y): " << levelnode.position.x << " " << levelnode.position.y << std::endl;
			//		}
			//	} 
			//	
			//	if (levelnode.position.x < player_motion.position.x) {
			//		// dist to left calculation
			//		float dist = sqrt(pow((levelnode.position.x - player_motion.position.x), 2) + pow(levelnode.position.y - player_motion.position.y, 2));
			//		if (dist < curr_best_dist) {
			//			curr_best_dist = dist;
			//			nearest_levelNode_pos = levelnode.position;
			//			nearest_left_node = levelnode;
			//			std::cout << "THIS IS THE POSITION OF THE nearest_left_node: (x, y): " << levelnode.position.x << " " << levelnode.position.y << std::endl;
			//		}
			//	}
			//	
			//}


			if (key == GLFW_KEY_UP) {
				player_motion.velocity.y = -player_speed;
			}
			else if (key == GLFW_KEY_DOWN) {
				player_motion.velocity.y = player_speed;
			}
			// just do left and right for now
			else if (key == GLFW_KEY_LEFT) {
				if (found_left_node && player_motion.position.x != leftmost_x) {
					
					// Move player to the nearest left node if not already at the leftmost node.
					player_motion.position.x = nearest_left_node.position.x;

					std::cout << "THIS IS left: " << nearest_left_node.level_number << std::endl;

					registry.players.components[i].level_num = nearest_left_node.level_number;
					/*remaining_distance_x = nearest_left_dist;
					player_motion.velocity.x = -player_speed;
					*/

					// TRYING TO MAKE A LOOP THAT WILL MAKE THE CHARACTER MOVE AND NOT JUST TELEPORT
					//std::cout << "I GOT HERE" << std::endl;
					//std::thread myThread([nearest_left_dist, &player_motion, &nearest_left_node]() {
					//	int update_nearest_left_dist = nearest_left_dist;
					//	std::cout << "Thread is running..." << std::endl;

					//	while (update_nearest_left_dist > 0) {
					//		std::cout << "Thread is running..." << std::endl;
					//		player_motion.position.x--;
					//		update_nearest_left_dist = abs(player_motion.position.x - nearest_left_node.position.x);

					//		std::cout << update_nearest_left_dist << std::endl;
					//		// Break condition for the loop, if necessary.
					//		// if (some_condition) break;
					//	}

					//});

					//std::cout << "I GOT HERE 2" << std::endl;
					//myThread.detach();
					//std::cout << "I GOT HERE 3" << std::endl;
				}
			}
			else if (key == GLFW_KEY_RIGHT) {
				if (found_right_node && player_motion.position.x != rightmost_x) {
					// Move player to the nearest right node if not already at the rightmost node.
					player_motion.position.x = nearest_right_node.position.x;
					std::cout << "THIS IS RIGHT: " << nearest_right_node.level_number << std::endl;
					registry.players.components[i].level_num = nearest_right_node.level_number;
					
				}
			}

		}

		/*if (remaining_distance_x == 0) {
			player_motion.velocity.x = 0;
		}
		else {
			remaining_distance_x = player_motion.position.x - 
			std::cout << "This value should go down: " << nearest_left_dist << std::endl;
		}*/


		/*else if (action == GLFW_RELEASE) {
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
		}*/
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
