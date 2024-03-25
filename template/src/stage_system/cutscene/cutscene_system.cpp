#include "cutscene_system.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

CutSceneSystem::CutSceneSystem() :
	cutscene_done(false), 
	skip_cutscene(false)
{}

void CutSceneSystem::init(StageSystem* stage_system_arg) {
	stage_system = stage_system_arg;
	cutscene_done = false;
	skip_cutscene = false;
}

void CutSceneSystem::handle_cutscene_timer(float elapsed_ms_since_last_update) {
	// TODO: Change cutscene done to TRUE after some set amount of time

}

void CutSceneSystem::handle_cutscene_keys(int key, int action) {
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_S) {
			if (cutscene_done) {
				// transition to turn-based
				std::cout << "GO TO TURN BASED\n";
				skip_cutscene = false;
				cutscene_done = false;
				handle_cutscene();
			}
		}

		if (key == GLFW_KEY_SPACE) {
			if (!skip_cutscene) {
				std::cout << "SKIP CUTSCENE\n";
				skip_cutscene = true;
			}
			else if (skip_cutscene) {
				std::cout << "CUTSCENE DONE\n";
				cutscene_done = true;
			}
		}
	}
}

void CutSceneSystem::handle_cutscene() {
	stage_system->set_stage(StageSystem::Stage::turn_based);
}