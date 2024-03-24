#include "main_menu_system.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

MainMenuSystem::MainMenuSystem() :
	selected_option(new_game)
{
}

void MainMenuSystem::init(StageSystem* stage_system_arg) {
	stage_system = stage_system_arg;
}

void MainMenuSystem::handle_menu_keys(int key, int action) {
	// order for main menu is:
	// new_game -> load_game -> options -> credits 
	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_UP:
				selected_option = static_cast<MenuOption>(((selected_option - 1) % (credits + 1) + (credits +1)) % (credits + 1));
				break;
			case GLFW_KEY_DOWN:
				selected_option = static_cast<MenuOption>((selected_option + 1) % (credits + 1));
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