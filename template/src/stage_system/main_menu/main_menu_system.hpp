#pragma once
#include "stage_system/stage_system.hpp"

#include "render_system.hpp"
#include "world_init.hpp"

// System for main menu
class MainMenuSystem
{
public:
	MainMenuSystem(); 

	enum MenuOption {
		new_game,
		load_game, 
		options,
		tutorials,
		credits
	};

	void init(StageSystem* stage_system_arg);

	void handle_option();

	void handle_menu_keys(int key, int action);

	void set_option(MenuOption target);

	MenuOption get_current_option() const {
		return this->selected_option;
	}
private:
	MenuOption selected_option;
	StageSystem* stage_system;

	std::map<MenuOption, Entity> menu_text_map;
	glm::vec3 selected_color = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 not_selected_color = glm::vec3(1.0, 1.0, 1.0);
};