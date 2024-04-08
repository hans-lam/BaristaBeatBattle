#include "main_menu_system.hpp"
#include "persistence/data_manager.hpp"

#include <GLFW/glfw3.h>
#include <iostream>



MainMenuSystem::MainMenuSystem() :
	selected_option(new_game)
{
}

void MainMenuSystem::init(StageSystem* stage_system_arg) {
	stage_system = stage_system_arg;
	selected_option = new_game;
	tutorial_on = false;
	const int basex = window_width_px / 2 - 200;
	menu_text_map[new_game] = createText("New Game", { basex,window_height_px/2 - 100 }, 1.5f, selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
	menu_text_map[load_game] = createText("Load Game", { basex,window_height_px / 2 - 150 }, 1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
	menu_text_map[save_game] = createText("Save Game", { basex,window_height_px / 2 - 200 }, 1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
	menu_text_map[tutorials] = createText("Tutorial", { basex,window_height_px / 2 - 250 }, 1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
	menu_text_map[credits] = createText("Credits", { basex,window_height_px / 2 - 300 }, 1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);

	main_menu_title = createText("BARISTA BEAT BATTLE", { basex - 300,window_height_px / 2 + 100 }, 3.f, glm::vec3(1.0, 1.0, 1.0), glm::mat4(1.0f), StageSystem::Stage::main_menu);
}

void MainMenuSystem::handle_menu_keys(int key, int action) {
	// order for main menu is:
	// new_game -> load_game -> options -> tutorials -> credits 
	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_UP:
				selected_option = static_cast<MenuOption>(((selected_option - 1) % (credits + 1) + (credits +1)) % (credits + 1));
				for (int i = new_game; i <= credits; i++) {
					MenuOption menu_option = static_cast<MenuOption>(i);
					registry.textRenderRequests.get(menu_text_map[menu_option]).color = not_selected_color;
				}
				registry.textRenderRequests.get(menu_text_map[selected_option]).color = selected_color;
				break;
			case GLFW_KEY_DOWN:
				selected_option = static_cast<MenuOption>((selected_option + 1) % (credits + 1));
				for (int i = new_game; i <= credits; i++) {
					MenuOption menu_option = static_cast<MenuOption>(i);
					registry.textRenderRequests.get(menu_text_map[menu_option]).color = not_selected_color;
				}
				registry.textRenderRequests.get(menu_text_map[selected_option]).color = selected_color;
				break;
			case GLFW_KEY_ENTER:
				handle_option();
				break;
		}
	}
}

void MainMenuSystem::handle_option()
{
	std::vector<Character*> curr_characters;
	Entity feedback;
	switch (selected_option) {
	case new_game:
		// set all texts to not be shown
		for (int i = new_game; i <= credits; i++) {
			MenuOption menu_option = static_cast<MenuOption>(i);
			TextRenderRequest& trr = registry.textRenderRequests.get(menu_text_map[menu_option]);
			trr.shown = false;
		}

		// go to overworld with default settings
		stage_system->set_stage(StageSystem::Stage::cutscene_before);
		break;
	case load_game:
		// popup game load option
		printf("load_game: ");
		data_manager->read_data("barista_beat_battle_save_data.json", stage_system);
		// TODO: show feedback to screen
		feedback = createText("Done Loading!", { 10,10 }, 0.9f, glm::vec3(1.0, 1.0, 1.0), glm::mat4(1.0f), StageSystem::Stage::main_menu);
		for (Entity& e : registry.persistanceFeedbackTimer.entities) {
			if (registry.textRenderRequests.has(e)) {
				registry.textRenderRequests.remove(e);
			}
		}
		registry.persistanceFeedbackTimer.clear();
		registry.persistanceFeedbackTimer.emplace(feedback);
		break;
	case save_game:
		// saves the game
		printf("save game: ");

		for (Entity character_entity : registry.characterDatas.entities) {

			
			Character* character_data = registry.characterDatas.get(character_entity).characterData;
			curr_characters.push_back(character_data);
			

		}

		data_manager->write_data("barista_beat_battle_save_data.json", 1, curr_characters, stage_system->is_london_recruited);

		// TODO: show feedback to screen
		for (Entity& e : registry.persistanceFeedbackTimer.entities) {
			if (registry.textRenderRequests.has(e)) {
				registry.textRenderRequests.remove(e);
			}
		}
		feedback = createText("Done Saving!", { 10,10 }, 0.9f, glm::vec3(1.0, 1.0, 1.0), glm::mat4(1.0f), StageSystem::Stage::main_menu);
		registry.persistanceFeedbackTimer.clear();
		registry.persistanceFeedbackTimer.emplace(feedback);
		break;
	case tutorials:
		if (tutorial_on == false) {
			tutorial_on = true;
			tutorial.push_back(createText("Welcome to our game! To get started, use the enter key and up down buttons", {10, 870}, 1.0f, glm::vec3(1.0, 0.4, 0.4), glm::mat4(1.0f), StageSystem::Stage::main_menu));
			tutorial.push_back(createText("1) use the enter key and up down buttons to make a selection", { 10, 840 }, 1.0f, glm::vec3(1.0, 0.4, 0.4), glm::mat4(1.0f), StageSystem::Stage::main_menu));
			tutorial.push_back(createText("2) click on \'NEW GAME\' ", { 10, 810 }, 1.0f, glm::vec3(1.0, 0.4, 0.4), glm::mat4(1.0f), StageSystem::Stage::main_menu));
			tutorial.push_back(createText("3) select a level on the overworld ", { 10, 780 }, 1.0f, glm::vec3(1.0, 0.4, 0.4), glm::mat4(1.0f), StageSystem::Stage::main_menu));
			tutorial.push_back(createText("      with the left and right buttons", { 10, 750 }, 1.0f, glm::vec3(1.0, 0.4, 0.4), glm::mat4(1.0f), StageSystem::Stage::main_menu));
		} else if (tutorial_on) {
			
			tutorial_on = false;
			for (Entity tut : tutorial) {
				if (registry.textRenderRequests.has(tut)) {
					registry.textRenderRequests.remove(tut);
				}
			}
			
			
		}
		printf("tutorials: "); 
		break;
	case credits:
		// popup credits screen
		printf("credits: ");
		break;
	default:
		printf("something fucked up\n");
		break;
	}
}

void MainMenuSystem::set_option(MenuOption target)
{
	selected_option = target;
}