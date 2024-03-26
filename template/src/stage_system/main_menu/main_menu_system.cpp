#include "main_menu_system.hpp"

#include <GLFW/glfw3.h>
#include <iostream>



MainMenuSystem::MainMenuSystem() :
	selected_option(new_game)
{
}

void MainMenuSystem::init(StageSystem* stage_system_arg) {
	stage_system = stage_system_arg;
	selected_option = new_game;
	const int basex = window_width_px / 2 - 200;
	menu_text_map[new_game] = createText("New Game", { basex,window_height_px/2 - 100 }, 1.5f, selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
	menu_text_map[load_game] = createText("Load Game", { basex,window_height_px / 2 - 150 }, 1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
	menu_text_map[options] = createText("Options", { basex,window_height_px / 2 - 200 }, 1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
	menu_text_map[tutorials] = createText("Tutorials", { basex,window_height_px / 2 - 250 }, 1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
	menu_text_map[credits] = createText("Credits", { basex,window_height_px / 2 - 300 }, 1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::main_menu);
}

void MainMenuSystem::handle_menu_keys(int key, int action) {
	// order for main menu is:
	// new_game -> load_game -> options -> credits 
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


		std::cout << selected_option << '\n';
	}
}

void MainMenuSystem::handle_option()
{
	switch (selected_option) {
	case new_game:
		// set all texts to not be shown
		for (int i = new_game; i <= credits; i++) {
			MenuOption menu_option = static_cast<MenuOption>(i);
			TextRenderRequest& trr = registry.textRenderRequests.get(menu_text_map[menu_option]);
			trr.shown = false;
		}

		// go to overworld with default settings
		stage_system->set_stage(StageSystem::Stage::overworld);
		break;
	case load_game:
		// popup game load option
		printf("load_game: ");
		break;
	case options:
		// popup options screen
		printf("options: ");
		break;
	case tutorials:
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