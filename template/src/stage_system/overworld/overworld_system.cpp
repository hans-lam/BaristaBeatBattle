#include "overworld_system.hpp"
#include "tiny_ecs_registry.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <world_init.hpp>
#include <thread>

// Justin: NOTE THAT SOME OF THE FUNCTOINALITY IS ALSO IN world_system.cpp


OverworldSystem::OverworldSystem() :
	overworld_tutorial(false),

	// THERE IS SOMETHING ABOUT THIS SO I COMMENTED THIS OUT:
	current_level(1)
{
}

void OverworldSystem::init(StageSystem* stage_system_arg) {
	stage_system = stage_system_arg;
	overworld_tutorial = false;
	current_level = 1;
	
	// I dont think this call works
	//create_overworld_levels();
	// std::cout << "IS THIS INIT BEING CALLED?????" << std::endl;
	
} 

bool OverworldSystem::step() {
	std::cout << "test" << std::endl;
	return true;
}

void OverworldSystem::handle_overworld_keys(int key, int action, float player_speed) {
	// Movement keys
	if (action == GLFW_PRESS && (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT || key == GLFW_KEY_UP || key == GLFW_KEY_DOWN)) {
		handle_player_movement(key, action, player_speed);
	} 
	// Attack key
	else if (action == GLFW_PRESS && key == GLFW_KEY_ENTER && !(registry.animations.has(registry.players.entities[0])) ) {		//player_attack();
		handle_level_selection();
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
			nearest_left_node;
			nearest_right_node;
			bool found_left_node = false;
			bool found_right_node = false;

			for (LevelNode& levelnode : registry.levelNode.components) {

				// Calculate distance from player to current node.
				float dist = sqrt(pow((levelnode.position.x - player_motion.position.x), 2) + pow((levelnode.position.y - player_motion.position.y), 2));
				dist_remaining = dist; // this will inform the updatePlayerVelocityFromTarget fuction, this will be normalized into the time argument
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
				//player_motion.velocity.y = -player_speed;
			}
			else if (key == GLFW_KEY_DOWN) {
				//player_motion.velocity.y = player_speed;
			}
			// just do left and right for now
			else if (key == GLFW_KEY_LEFT) {
				if (found_left_node && player_motion.position.x != leftmost_x && !registry.animations.has(registry.players.entities[0])) {
					
					// Move player to the nearest left node if not already at the leftmost node.
					/*player_motion.position.x = nearest_left_node.position.x;
					current_level = nearest_left_node.level_number;*/

					// std::cout << "THIS IS left: " << nearest_left_node.level_number << std::endl;

					// This next line actually just handles the animation because the movement is just an assignment of position 
					// values, not the velocity thing anymore. This is very hacky and should be fixed at some point. 
					player_motion.velocity.x = -player_speed;

					prev_level = current_level;
					prev_node.position = player_motion.position;

					current_level = nearest_left_node.level_number; 
					nearest_node = nearest_left_node;
					

					registry.players.components[i].level_num = nearest_left_node.level_number;
					
					remaining_distance_x = nearest_left_dist;

					dist_remaining = nearest_left_dist;

					
					stored_static_dist_remaining = nearest_left_dist;

					// Add to animation registry:

					addToAnimation(registry.players.entities[0]);
					
				}
			}
			else if (key == GLFW_KEY_RIGHT) {
				if (found_right_node && player_motion.position.x != rightmost_x && !registry.animations.has(registry.players.entities[0])) {
					// Move player to the nearest right node if not already at the rightmost node.
					//player_motion.position.x = nearest_right_node.position.x;
					
					// This next line actually just handles the animation because the movement is just an assignment of position 
					// values, not the velocity thing anymore. This is very hacky and should be fixed at some point. 
					player_motion.velocity.x = player_speed;

					prev_level = current_level;
					prev_node.position = player_motion.position;

					current_level = nearest_right_node.level_number;
					nearest_node = nearest_right_node;

					registry.players.components[i].level_num = nearest_right_node.level_number;

					remaining_distance_y = nearest_right_dist;

					dist_remaining = nearest_right_dist;

					// TESTING:

					stored_static_dist_remaining = nearest_left_dist;

					addToAnimation(registry.players.entities[0]);
					
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

/*
void OverworldSystem::player_attack() {
	for (uint i = 0; i < registry.players.size(); i++) {
		Entity entity = registry.players.entities[i];
		if (!registry.attackTimers.has(entity)) {
			registry.attackTimers.emplace(registry.players.entities[i]);
		}
	}
}
*/

int my_test_it = 1;
void OverworldSystem::handle_level_selection() {
	
	for (Entity entity : registry.levelNode.entities) {
		registry.remove_all_components_of(entity);
	}

	

	std::cout << " handle level selection " << my_test_it++ << std::endl;
	stage_system->set_stage(StageSystem::Stage::cutscene, current_level);
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

Entity OverworldSystem::addToAnimation(Entity entity) {

	// Initialize the animation:
	auto& animation = registry.animations.emplace(entity);
	animation.start_pos = prev_node.position;
	animation.end_pos = nearest_node.position;

	animation.total_ms = 1500.0f;
	animation.current_ms = 0.0f;;


	return entity;
}

void OverworldSystem::update_time(float elapsed_time) {
	curr_time_global_var = elapsed_time;
	
}


void OverworldSystem::updatePlayerVelocityTowardsTarget(float elapsed_ms) {


	// There used to be functionality that updated position based on getBezierPath, the bezier curve function is now in step() of world_system.cpp
	
		// THIS IS REALLY IMPORTANT TO SNAP THE CHARACTER TO A THING:
		if (0 > registry.motions.get(registry.players.entities[0]).velocity.x) {
			// check using nearest_node from the overworld_system
			float x_pos_near_left = this->nearest_left_node.position.x;

			// move the player based on bezier:
			float step_seconds = elapsed_ms / 1000.f;
			// registry.motions.get(registry.players.entities[0]).position.x = getBezierPath()
			if (10 > abs((int)x_pos_near_left - (int)registry.motions.get(registry.players.entities[0]).position.x)) {

				registry.motions.get(registry.players.entities[0]).velocity.x = 0;
				registry.motions.get(registry.players.entities[0]).position.x = x_pos_near_left;

			}
		}
		else if (0 < registry.motions.get(registry.players.entities[0]).velocity.x) {
			// check using nearest_node from the overworld_system
			float x_pos_near_right = this->nearest_right_node.position.x;
			if (10 > abs((int)x_pos_near_right - (int)registry.motions.get(registry.players.entities[0]).position.x)) {
				registry.motions.get(registry.players.entities[0]).velocity.x = 0;
				registry.motions.get(registry.players.entities[0]).position.x = x_pos_near_right;

			}
		}
		else {
			// std::cout << "REACHED ELSE CASE" << std::endl;
		}

		//if (0 > registry.motions.get(registry.players.entities[0]).velocity.x) {
		//	// check using nearest_node from the overworld_system
		//	float x_pos_near_left = this->nearest_left_node.position.x;

		//	// move the player based on bezier:
		//	float step_seconds = elapsed_ms / 1000.f;
		//	// registry.motions.get(registry.players.entities[0]).position.x = getBezierPath()

		//	if (0 == abs((int)x_pos_near_left - (int)registry.motions.get(registry.players.entities[0]).position.x)) {


		//		registry.motions.get(registry.players.entities[0]).velocity.x = 0;
		//		registry.motions.get(registry.players.entities[0]).position.x = x_pos_near_left;

		//	}
		//}
		//else if (0 < registry.motions.get(registry.players.entities[0]).velocity.x) {
		//	// check using nearest_node from the overworld_system
		//	float x_pos_near_right = this->nearest_right_node.position.x;
		//	if (0 ==  abs((int)x_pos_near_right - (int)registry.motions.get(registry.players.entities[0]).position.x)) {
		//		registry.motions.get(registry.players.entities[0]).velocity.x = 0;
		//		registry.motions.get(registry.players.entities[0]).position.x = x_pos_near_right;

		//	}
		//}
		//else {
		//	// std::cout << "REACHED ELSE CASE" << std::endl;
		//}
}

// This function takes in 2 points and a float time in range [0, 1] and returns the position in a path  
vec2 OverworldSystem::getBezierPath(float start_x, float start_y, float end_x, float end_y, float time) {

	// create 4 new values to represent the 2 new points to make this cubic bezier curve:

	// The x values will be equal to the midpoint of start_x and end_x:
	float up_x = (start_x + end_x) / 2.0;
	float down_x = (start_x + end_x) / 2.0;

	// The y values of up is just the higher of the two (assuming the axes are from bottom left. Which it is.
	float up_y = max(start_y, end_y) + 200;
	float down_y = min(start_y, end_y) - 200;

	// Calculate bezier curve at t:

	float retval_x;
	float retval_y;

	/*Consider 4 cases, 2 of them go from down to up, and 2 of them go from left to right:
	BUT we only care about where the 2 generated points lie, we have to check:
	1. If start_y is lower/less than end_y, we will say that the "down" point is the second
	   bezier point in the order of operation.
	2. If end_y is lower/less than start_y, the "down" point is the third bezier point. The
	   "up" point then becomes the second bezier curve because start_y is above end_y.
	*/

	// vec2 first_bezier; is always just start_x and start_y
	vec2 second_bezier = vec2( 0.0f, 0.0f );
	vec2 third_bezier = vec2(0.0f, 0.0f);
	// vec2 fourth_bezier; is always just end_x and end_y


	if (start_y < end_y) {
		// second_bezier.x corresponds with the x value 
		second_bezier.x = down_x;
		second_bezier.y = down_y;

		third_bezier.x = up_x;
		third_bezier.y = up_y;
	}
	else if (end_y < start_y) {
		second_bezier.x = up_x;
		second_bezier.y = up_y;

		third_bezier.x = down_x;
		third_bezier.y = down_y;
	}

	

	// getting the x value
	retval_x = std::pow(1 - time, 3) * start_x +
		3 * std::pow(1 - time, 2) * time * second_bezier.x +
		3 * (1 - time) * std::pow(time, 2) * third_bezier.x +
		std::pow(time, 3) * end_x;

	retval_y = std::pow(1 - time, 3) * start_y +
		3 * std::pow(1 - time, 2) * time * second_bezier.y +
		3 * (1 - time) * std::pow(time, 2) * third_bezier.y +
		std::pow(time, 3) * end_y;

	vec2 retval { retval_x, retval_y };


	return retval;
}

