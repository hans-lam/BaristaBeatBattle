#include "combat_system.hpp"
#include "common.hpp"

const int BASE_X_VALUE = 100;

CombatSystem::CombatSystem() :
	selected_level(level_one),
	turn_based_tutorial(false)
{}

void CombatSystem::init(StageSystem* stage_system_arg, TurnBasedSystem* turn_based_arg) {
	stage_system = stage_system_arg;
	turn_based = turn_based_arg;
	// This can change depending on how we implement saving/loading
	selected_level = level_one;
}

CombatSystem::SoundMapping CombatSystem::handle_turnbased_keys(int key, int action) {
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_T) {
			handle_tutorial();
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			// handle changing of menu here
			return handle_menu(key);
		}
		if (key == GLFW_KEY_ENTER) {
			return handle_selection();
		}
	}
	return SoundMapping::no_sound;
}

void CombatSystem::handle_level(RenderSystem* renderer) {
	vec2 base_ally_position = { BASE_X_VALUE, window_height_px - 200 };
	vec2 base_enemy_position = { window_width_px - 100, window_height_px - 200 };
	Level* level;
	switch (selected_level) {
	case level_one:
		level = level_factory->construct_level_one(renderer, base_ally_position, base_enemy_position);
		init_combat_data_for_level(renderer, level);
		break;
	case level_two:
		// Need to switch this up once more create levels are implemented
		level = level_factory->construct_level_two(renderer, base_ally_position, base_enemy_position);
		init_combat_data_for_level(renderer, level);
		break;
	case level_three:
		level = level_factory->construct_level_three(renderer, base_ally_position, base_enemy_position);
		init_combat_data_for_level(renderer, level);
		break;
	case level_four:
		level = level_factory->construct_level_four(renderer, base_ally_position, base_enemy_position);
		init_combat_data_for_level(renderer, level);
		break;
	case level_five:
		level = level_factory->construct_level_five(renderer, base_ally_position, base_enemy_position);
		init_combat_data_for_level(renderer, level);
		break;
	case level_six:
		level = level_factory->construct_level_six(renderer, base_ally_position, base_enemy_position);
		init_combat_data_for_level(renderer, level);
		break;
	case level_seven:
		level = level_factory->construct_level_seven(renderer, base_ally_position, base_enemy_position);
		init_combat_data_for_level(renderer, level);
		break;
	}

	turn_based->start_encounter(level);
}

CombatSystem::SoundMapping CombatSystem::handle_selection() {
	Entity active_char_entity = turn_based->get_active_character();
	CombatSystem::SoundMapping play_sound = SoundMapping::no_sound;

	if (active_char_entity != emptyEntity) {
		// Get active option
		for (Entity menu_entity : registry.menu.entities) {
			Menu& menu = registry.menu.get(menu_entity);

			// found correct menu
			if (menu.associated_character == active_char_entity) {
				Entity correctOption = menu.activeOption;
				MenuOption& opComponent = registry.menuOptions.get(correctOption);

				if (opComponent.option == "attack") {
					handle_attack(active_char_entity, "Basic Attack");
				}
				else if (opComponent.option == "rest") {
					// TODO USE REST ABILITY

					play_sound = handle_attack(active_char_entity, "rest");

				}
				else if (opComponent.option == "pour it") {
					stage_system->set_stage(StageSystem::Stage::minigame);
				}
			}
		}
	}

	return play_sound;
}

void CombatSystem::handle_combat_over() {
	for (Entity entity : registry.turnBased.entities) {
		if (registry.turnBasedEnemies.has(entity)) {
			registry.remove_all_components_of(entity); 
		}

		// TODO: If we need to remove player/party members after each turn based encounter, do that here
		if (registry.partyMembers.has(entity)) {
			registry.remove_all_components_of(entity);
		}
		if (registry.players.has(entity)) {
			registry.remove_all_components_of(entity);
		}
	}
}

void CombatSystem::handle_minigame_attack(Entity active_char_entity, int score) {
	Character* active_char = registry.characterDatas.get(active_char_entity).characterData;
	Ability* minigame_ability = active_char->get_ability_by_name("cool");
	minigame_ability->set_power(score);

	handle_attack(active_char_entity, "cool");
}

CombatSystem::SoundMapping CombatSystem::handle_attack(Entity active_char_entity, std::string ability) {
	Character* active_char = registry.characterDatas.get(active_char_entity).characterData;
	CombatSystem::SoundMapping play_sound = SoundMapping::no_sound;

	if (!out_of_combat) {
		play_sound = attack_sound;

		Character* target;

		if (ability == "rest") {
			target = registry.characterDatas.get(active_char_entity).characterData;
		}
		else {
			Entity enemy_target_entity = registry.turnBasedEnemies.entities[0];
			target = registry.characterDatas.get(enemy_target_entity).characterData;
		}


		int is_game_over = turn_based->process_character_action(active_char->get_ability_by_name(ability), active_char, { target });

		if (is_game_over != 0) {
			// delete all enemies	
			handle_combat_over();
			// move selected level to next level
			selected_level = static_cast<CombatLevel>((selected_level + 1) % (level_seven + 1));
			stage_system->set_stage(StageSystem::Stage::overworld);
		}
	}

	return play_sound;
}

CombatSystem::SoundMapping CombatSystem::handle_menu(int key) {
	Entity active_char_entity = turn_based->get_active_character();
	CombatSystem::SoundMapping play_sound = SoundMapping::no_sound;

	if (active_char_entity != emptyEntity) {

		Character* active_char = registry.characterDatas.get(active_char_entity).characterData;

		for (Entity menu_entity : registry.menu.entities) {
			Menu& menu = registry.menu.get(menu_entity);
			int arrayLen = std::end(menu.options) - std::begin(menu.options);
			int index = -1;
			for (int i = 0; i < arrayLen; i++) {
				if (menu.activeOption == menu.options[i]) {
					index = i;
					break;
				}
			}

			if (menu.associated_character == active_char_entity) {
				if (key == GLFW_KEY_UP) {
					if (index > 0) {
						menu.activeOption = menu.options[index - 1];
					}
					else {
						menu.activeOption = menu.options[(arrayLen - 1)];
					}
				}
				else if (key == GLFW_KEY_DOWN) {
					if (index < (arrayLen - 1)) {
						menu.activeOption = menu.options[index + 1];
					}
					else {
						menu.activeOption = menu.options[0];
					}
				}
				play_sound = SoundMapping::change_sound;
				break;
			}
		}
	}

	return play_sound;
}

void CombatSystem::handle_tutorial() {
	for (Entity entity : registry.tutorials.entities) {
		if (registry.turnBased.has(entity)) {
			RenderRequest& rr = registry.renderRequests.get(entity);

			if (turn_based_tutorial) {
				// set the shown to false for the tutorial board
				rr.shown = false;
			}
			else {
				rr.shown = true;
			}

			turn_based_tutorial = !turn_based_tutorial;
			break;
		}
	}
}

void CombatSystem::init_combat_data_for_level(RenderSystem* renderer, Level* level) {

	int j = 0;
	for (Entity party_member_entity : level->allies) {
		// Creating Menu entity 
		vec2 menu_pos = { BASE_X_VALUE + 200, window_height_px - (j + 1) * 200 };
		Entity menuEnt = createMenu(renderer, menu_pos, party_member_entity);
		Menu& menu = registry.menu.get(menuEnt);

		j++;
	}

}

void CombatSystem::handle_turn_rendering() {
	// handle rendering menus
	Entity active_char_entity = turn_based->get_active_character();

	if (active_char_entity != emptyEntity) {
		Character* active_char = registry.characterDatas.get(active_char_entity).characterData;

		// setting player color 
		for (Entity entity : registry.partyMembers.entities) {
			vec3& color = registry.colors.get(entity);

			if (entity == active_char_entity) {
				// change current player character to red
				color = { 1.0f, 0.f, 0.f };
			}
			else {
				// return player character color to normal
				color = { 1, 0.8f, 0.8f };
			}
		}

		// Setting menu's active 
		for (Entity menu_entity : registry.menu.entities) {
			Menu& menu = registry.menu.get(menu_entity);
			Entity attack = menu.options[0];
			Entity rest = menu.options[1];
			Entity pourIt = menu.options[2];
			MenuOption& atk = registry.menuOptions.get(attack);
			MenuOption& rst = registry.menuOptions.get(rest);
			MenuOption& prt = registry.menuOptions.get(pourIt);

			if (menu.associated_character == active_char_entity) {
				if (!registry.renderRequests.has(attack)) {
					RenderRequest& rr = registry.renderRequests.insert(
						attack,
						{ TEXTURE_ASSET_ID::ATTACKBUTTON, // TEXTURE_COUNT indicates that no txture is needed
						EFFECT_ASSET_ID::TEXTURED,
						GEOMETRY_BUFFER_ID::SPRITE });
					rr.shown = true;
				}
				if (!registry.renderRequests.has(rest)) {
					RenderRequest& rr = registry.renderRequests.insert(
						rest,
						{ TEXTURE_ASSET_ID::RESTBUTTON, // TEXTURE_COUNT indicates that no txture is needed
						EFFECT_ASSET_ID::TEXTURED,
						GEOMETRY_BUFFER_ID::SPRITE });
					rr.shown = true;
				}
				if (!registry.renderRequests.has(pourIt)) {
					RenderRequest& rr = registry.renderRequests.insert(
						pourIt,
						{ TEXTURE_ASSET_ID::ITEMBUTTON, // TEXTURE_COUNT indicates that no txture is needed
						EFFECT_ASSET_ID::TEXTURED,
						GEOMETRY_BUFFER_ID::SPRITE });
					rr.shown = true;
				}
			}
			else {
				if (registry.renderRequests.has(attack)) {
					registry.renderRequests.remove(attack);
				}
				if (registry.renderRequests.has(rest)) {
					registry.renderRequests.remove(rest);
				}
				if (registry.renderRequests.has(pourIt)) {
					registry.renderRequests.remove(pourIt);
				}
			}

			// handle color change of option 
			for (Entity entity : menu.options) {
				vec3& color = registry.colors.get(entity);

				if (entity == menu.activeOption) {
					color.x = 0.8f;
				}
				else {
					color.x = 1;
				}
			}
		}
	}
}