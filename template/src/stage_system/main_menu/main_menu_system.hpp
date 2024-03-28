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
		save_game,
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

	bool tutorial_on;

	MenuOption selected_option;
	StageSystem* stage_system;
	std::vector<Entity> tutorial;
	std::map<MenuOption, Entity> menu_text_map;
	Entity main_menu_title;
	glm::vec3 selected_color = glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 not_selected_color = glm::vec3(1.0, 1.0, 1.0);
};