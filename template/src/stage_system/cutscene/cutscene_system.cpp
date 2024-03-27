#include "cutscene_system.hpp"
#include "common.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

CutSceneSystem::CutSceneSystem() :
	cutscene_done(false), 
	skip_cutscene(false)
{}

void CutSceneSystem::init(StageSystem* stage_system_arg, RenderSystem * renderer) {
	stage_system = stage_system_arg;
	cutscene_done = false;
	skip_cutscene = false;
}

void CutSceneSystem::handle_cutscene_render(RenderSystem * renderer) {
	// TODO: Change cutscene done to TRUE after some set amount of time
	//Entity textBox;

	Entity textBox;
	Entity char1;
	Entity char2;
	Entity text; 
	switch (cutscene_slide) {
	case 1: 
		textBox = create_cutscene_text_box(renderer, vec2(750,700 ), vec2(600, 120), "New Game",  1.5f, glm::vec3(0.12941176470588237, 0.06274509803921569, 0.06666666666666667), glm::mat4(1.0f), StageSystem::Stage::cutscene);

		

		//
		break;
	case 2: 
		// textBox = create_cutscene_text_box(renderer, vec2(0, 0));
		break;
	case 3:
		 //textBox = create_cutscene_text_box(renderer, vec2(0, 0));
		break;
	case 4:
		 //textBox = create_cutscene_text_box(renderer, vec2(0, 0));
		break;
	}

	


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
		if (key == GLFW_KEY_N) {
			if (!cutscene_done) {
				// transition to turn-based
				cutscene_slide++;
			}
			if (cutscene_slide == 5) {
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

