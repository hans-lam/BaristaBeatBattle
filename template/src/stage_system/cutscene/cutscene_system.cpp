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
	Entity instructionsText= createText("Press N to continue cutscene", { 10, 790 }, 1.5f, glm::vec3(1.0,1.0,1.0), glm::mat4(1.0f), StageSystem::Stage::cutscene);
	Entity instructionsText2 = createText("Press SPACE twice to skip cutscene", { 10, 740 }, 1.5f, glm::vec3(1.0, 1.0, 1.0), glm::mat4(1.0f), StageSystem::Stage::cutscene);


	switch (cutscene_slide) {
	case 1: 
		
		textBox = create_cutscene_text_box(renderer, 1, vec2(750,700 ), vec2(415, 250), " Welcome to Brew Battle Cafe, a bustling place", "filled with beats, baristas, and battles alike  in the never " , " ending battle to prove their best in the age - old rivalry between coffee and tea!", .56f, glm::vec3(0.12941176470588237, 0.06274509803921569, 0.06666666666666667), glm::mat4(1.0f), StageSystem::Stage::cutscene);
		//
		break;
	case 2: 
		

		textBox = create_cutscene_text_box(renderer, 2,  vec2(750, 700), vec2(415, 250), "TEST", "filled with beats, baristas, and battles alike  in the never ", " ending battle to prove their best in the age - old rivalry between coffee and tea!", .56f, glm::vec3(0.12941176470588237, 0.06274509803921569, 0.06666666666666667), glm::mat4(1.0f), StageSystem::Stage::cutscene);
		
		char1 = createCharPic(renderer, vec2(750, 500));
		// textBox = create_cutscene_text_box(renderer, vec2(0, 0));
		break;
	case 3:
		//registry.renderRequests.get(textBox).shown = false;
		textBox = create_cutscene_text_box(renderer, 3, vec2(750, 700), vec2(415, 250), "TEST", "filled with beats, baristas, and battles alike  in the never ", " ending battle to prove their best in the age - old rivalry between coffee and tea!", .56f, glm::vec3(0.12941176470588237, 0.06274509803921569, 0.06666666666666667), glm::mat4(1.0f), StageSystem::Stage::cutscene);
		

		char1 = createCharPic(renderer, vec2(750, 500));
		//textBox = create_cutscene_text_box(renderer, vec2(0, 0));
		break;
	}
	if (cutscene_done) {
		// transition to turn-based
		std::cout << "GO TO TURN BASED\n";
		skip_cutscene = false;
		cutscene_done = false;
		handle_cutscene();
	}
	


}

Entity CutSceneSystem::createCharPic(RenderSystem * renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 700.f;
	motion.scale.y *= -1; // point front to the right

	// Create and (empty) Chicken component to be able to refer to all eagles
	registry.players.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });
	rr.shown = true;


	registry.cutscenes.emplace(entity);

	return entity;
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
				cutscene_slide = 1;
			}
		}
		if (key == GLFW_KEY_N) {
			if (!cutscene_done) {
				// transition to turn-based
				cutscene_slide++;
				printf("You entered: %d", cutscene_slide);
			}
			if (cutscene_slide >= 4) {
				std::cout << "GO TO TURN BASED\n";
				skip_cutscene = false;
				cutscene_done = false;
				handle_cutscene();
				cutscene_slide = 1;
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

