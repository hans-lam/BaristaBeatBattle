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
	level_factory = new LevelFactory();
}

CombatSystem::SoundMapping CombatSystem::handle_turnbased_keys(int key, int action) {
	if (out_of_combat) return SoundMapping::no_sound;

	if (action == GLFW_RELEASE && key == GLFW_KEY_T) {
		handle_tutorial();
	}

	if (action == GLFW_RELEASE && (key == GLFW_KEY_DOWN || key == GLFW_KEY_UP) ) {
		return handle_menu(key);
	}

	if (action == GLFW_RELEASE && key == GLFW_KEY_ENTER) {
		return handle_selection();
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
					break;
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
	
	for (Entity entity : registry.partyMembers.entities) {
		registry.remove_all_components_of(entity);
	}

	for (Entity entity : registry.turnBasedEnemies.entities) {
		registry.remove_all_components_of(entity);
	}


	for (Entity entity : registry.healthBarFills.entities) {
		registry.remove_all_components_of(entity);
	}

	for (Entity entity : registry.healthOutlines.entities) {
		registry.remove_all_components_of(entity);
	}

	for (Entity entity : registry.menu.entities) {
		registry.remove_all_components_of(entity);
	}

	for (Entity entity : registry.menuOptions.entities) {
		registry.remove_all_components_of(entity);
	}

	for (Entity entity : registry.injuryTimers.entities) {
		registry.remove_all_components_of(entity);
	}

	for (Entity entity : registry.turnBasedEnemies.entities) {
		registry.remove_all_components_of(entity);
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

			// if level four ended with an enemy still there then london is required
			// For M4 refactor this to be a proper use of flagging
			if (is_game_over == 1 && selected_level == level_four && registry.turnBasedEnemies.size() == 1) {
				flag_progression->is_london_recruited = true;
			}

			// delete all enemies	
			handle_combat_over();
			// move selected level to next level
			//selected_level = static_cast<CombatLevel>((selected_level + 1) % (level_seven + 1));
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
	// remove the level texts
	for (Entity& e : registry.turnBased.entities) {
		if (registry.textRenderRequests.has(e)) {
			registry.textRenderRequests.remove(e);
		}
	}
	Entity level_text = createText("Level " + std::to_string(level->level_number), {window_width_px / 2 - 100, window_height_px - 50}, 1.3f, vec3(1.0f, 1.0f, 1.0f), mat4(1.0f), StageSystem::Stage::turn_based);
	registry.textRenderRequests.get(level_text).shown = true;

	int j = 0;
	float dy = -75.f;

	for (Entity party_member_entity : level->allies) {
		// Creating Menu entity 
		vec2 menu_pos = { BASE_X_VALUE + 300, window_height_px - (j + 1) * 200 };
		Entity menuEnt = createMenu(renderer, menu_pos, party_member_entity);
		Menu& menu = registry.menu.get(menuEnt);

		j++;

		vec2 posn = registry.motions.get(party_member_entity).position;
		create_health_bar_outline(renderer, posn + vec2(25.f, dy));
		create_health_bar_fill(renderer, posn + vec2(50.f, dy), party_member_entity);
	}

	for (Entity enemy_member : level->enemies) {
		vec2 posn = registry.motions.get(enemy_member).position;
		create_health_bar_outline(renderer, posn + vec2(-25.f, dy));
		create_health_bar_fill(renderer, posn + vec2(0.f, dy), enemy_member);
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